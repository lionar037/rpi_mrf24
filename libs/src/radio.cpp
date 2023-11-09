
#include <string>			// memset()

#include "radioAddress.h"
#include "radio.h"
// globals


namespace MRF24J40{


MRF24J40_STATUS volatile RadioStatus;						// radio state
uint8_t volatile RXBuffer[PACKET_BUFFERS][RX_BUFFER_SIZE];	// rx packet buffers 

PACKET Tx, Rx;	
				// structures describing transmitted and received packets

	// this combines memcpy with incrementing the source point.  It copies bytewise, allowing it to copy to/from unaligned addresses
	
	unsigned char* Radio::readBytes(unsigned char* dstPtr, unsigned char* srcPtr, unsigned int count)
	{
		while(count--)
			*dstPtr++ = *srcPtr++;

		return srcPtr;
	}



	// writes count consecutive bytes from source into consecutive FIFO slots starting at "register".  Returns next empty register #.
	uint8_t Radio::toTXfifo(uint16_t reg, uint8_t* source, uint8_t count)
	{
		while(count--)
			write_long(reg++,*source++);

		return reg;
	}

// warm start radio hardware, tunes to Channel.  Takes about 0.37 ms on PIC32 at 20 MHz, 10 MHz SPI hardware clock
// on return, 0=no radio hardare, 1=radio is reset
bool Radio::initMrf24j40(void)
{
	uint8_t i;
	uint32_t radioReset = ReadCoreTimer();	// record time we started the reset procedure

	RadioStatus.ResetCount++;

	RadioStatus.TX_BUSY = 0;			// tx is not busy after reset
	RadioStatus.TX_FAIL = 1;			// if we had to reset, consider last packet (if any) as failed
	RadioStatus.TX_PENDING_ACK = 0;		// not pending an ack after reset
	RadioStatus.SLEEPING = 0;			// radio is not sleeping

	/* do a soft reset */
	write_short(WRITE_SOFTRST,0x07);		// reset everything (power, baseband, MAC) (also does wakeup if in sleep)
	do
	{
		i = read_short(READ_SOFTRST);

		if (CtTicksSince(radioReset) > MS_TO_CORE_TICKS(50))		// if no reset in a reasonable time
			return false;												// then there is no radio hardware
	}
	while((i&0x07) != (uint8_t)0x00);   	// wait for hardware to clear reset bits

	write_short(WRITE_RXFLUSH,0x01);		// flush the RX fifo, leave WAKE pin disabled

	RadioSetAddress(RadioStatus.MyShortAddress, RadioStatus.MyLongAddress, RadioStatus.MyPANID);

	write_long(RFCTRL0,0x03);			// RFOPT=0x03
	write_long(RFCTRL1,0x02);			// VCOOPT=0x02, per datasheet
	write_long(RFCTRL2,0x80);			// PLL enable
	write_long(RFCTRL3, TX_POWER);		// set transmit power
	write_long(RFCTRL6,0x90);			// TXFILter on, 20MRECVR set to < 3 mS
	write_long(RFCTRL7,0x80);			// sleep clock 100 kHz internal
	write_long(RFCTRL8,0x10);			// RFVCO to 1

	write_long(SCLKDIV, 0x21);			// CLKOUT disabled, sleep clock divisor is 2

	write_short(WRITE_BBREG2,0x80);		// CCA energy threshold mode
	write_short(WRITE_BBREG6,0x40);		// RSSI on every packet
	write_short(WRITE_RSSITHCCA,0x60);		// CCA threshold ~ -69 dBm

	#if defined(ENABLE_PA_LNA)
		write_long(TESTMODE, 0x0F);		// setup for PA_LNA mode control
	#endif

	write_short(WRITE_FFOEN, 0x98);		// PACON2, per datasheet init
	write_short(WRITE_TXPEMISP, 0x95);  	// TXSTBL; RFSTBL=9, MSIFS-5

	while((read_long(RFSTATE)&0xA0) != 0xA0);	// wait till RF state machine in RX mode

	write_short(WRITE_INTMSK,0b11110110);	// INTCON, enabled=0. RXIE and TXNIE only enabled.

	// Make RF communication stable under extreme temperatures
	write_long(RFCTRL0, 0x03);			// this was previously done above
	write_long(RFCTRL1, 0x02);			// VCCOPT - whatever that does...

	SetChannel(RadioStatus.Channel);	// tune to current radio channel

	#ifdef TURBO_MODE					// propriatary TURBO_MODE runs at 625 kbps (vs. 802.15.4 compliant 250 kbps)
		write_short(WRITE_BBREG0, 0x01);	// TURBO mode enable
		write_short(WRITE_BBREG3, 0x38);	// PREVALIDTH to turbo optimized setting
		write_short(WRITE_BBREG4, 0x5C);	// CSTH carrier sense threshold to turbo optimal
	#endif

	write_short(WRITE_RFCTL,0x04);			// reset RF state machine
	write_short(WRITE_RFCTL,0x00);			// back to normal operation

	// now delay at least 192 uS per datasheet init

	return true ;
}

// on return, 1=radio is setup, 0=there is no radio
bool Radio::Init(void)					// cold start radio init
{
	bool radio;

	memset((void*)&RadioStatus, 0, sizeof(RadioStatus));

	RadioStatus.MyPANID 		= MY_PAN_ID;
	RadioStatus.MyShortAddress 	= MY_SHORT_ADDRESS;
	RadioStatus.MyLongAddress  	= MY_LONG_ADDRESS;

	RadioStatus.Channel = 24;			// start at channel 11

	radio = initMrf24j40();				// init radio hardware, tune to RadioStatus.Channel

	RFIE = 1;							// enable radio interrupts

	return radio;
}

// set short address and PANID
void Radio::SetAddress(const uint16_t shortAddress, const uint64_t longAddress, const uint16_t panID)
{
        write_short(WRITE_SADRH, (shortAddress >> 8)& 0xff);
        write_short(WRITE_SADRL, shortAddress & 0xff);

		write_short(WRITE_PANIDH, (panID >> 8)& 0xff);
		write_short(WRITE_PANIDL, panID & 0xff);

	for(uint8_t i = 0 ; i<sizeof(longAddress) ; i++)	// program long MAC address
		write_short(WRITE_EADR0+i*2,(longAddress>>i)&0xff);

	RadioStatus.MyPANID 		= panID;
	RadioStatus.MyShortAddress 	= shortAddress;
	RadioStatus.MyLongAddress  	= longAddress;
}

// Set radio channel.  Returns with success/fail flag.
bool Radio::SetChannel(const uint8_t channel)
{
	if( channel < 11 || channel > 26)
		return false;

	#if defined(ENABLE_PA_LNA)	// Permitted band is 2400 to 2483.5 MHz.
		if( channel == 26 )		// Center Freq. is 2405+5(k-11) MHz, for k=channel 11 to 26
			return false;		// max output is 100mW (USA)
	#endif						// rolloff is not steep enough to avoid 2483.5 from channel 26 center of 2480 MHz at full MB power

	RadioStatus.Channel = channel;
	write_long(RFCTRL0,((channel-11)<<4)|0x03);
	write_short(WRITE_RFCTL,0x04);	// reset RF state machine
	write_short(WRITE_RFCTL,0x00);	// back to normal

	return true;
}

// Put the RF transceiver into sleep or wake it up
// Radio power, MRF24J40MB - ENABLE_PA_LNA on:
//	RX:  	28.4 mA
//	TX: 	65.8 mA (as fast as I can xmit; nominal peak 130 mA)
//	Sleep:	0.245 mA (spec is 5 uA with 'sleep clock disabled'; setting register 0x211 to 0x01 doesn't seem to help)
// Note that you can in practice turn the radio power off completely for short periods (with a MOSFET) and then do a warm start.
void Radio::SetSleep(uint8_t powerState)
{
	if (powerState)
	{
		#if defined(ENABLE_PA_LNA)
			write_long(TESTMODE, 0x08);      // Disable automatic switch on PA/LNA
			write_short(WRITE_GPIODIR, 0x0F);	// Set GPIO direction to OUTPUT (control PA/LNA)
			write_short(WRITE_GPIO, 0x00);     // Disable PA and LNA
		#endif

		write_short(WRITE_SOFTRST, 0x04);		// power management reset to ensure device goes to sleep
		write_short(WRITE_WAKECON,0x80);		// WAKECON; enable immediate wakeup
		write_short(WRITE_SLPACK,0x80);		// SLPACK; force radio to sleep now

		RadioStatus.SLEEPING = 1;			// radio is sleeping
	}	
	else
		initMrf24j40();		// could wakeup with WAKE pin or by toggling REGWAKE (1 then 0), but this is simpler
}

// Do a single (128 us) energy scan on current channel.  Return RSSI.
uint8_t Radio::EnergyDetect(void)
{
	uint8_t RSSIcheck;

	#if defined(ENABLE_PA_LNA)
		write_long(TESTMODE, 0x08);          // Disable automatic switch on PA/LNA
		write_short(WRITE_GPIODIR, 0x0F);      // Set GPIO direction to OUTPUT (control PA/LNA)
		write_short(WRITE_GPIO, 0x0C);         // Enable LNA, disable PA
	#endif

	write_short(WRITE_BBREG6, 0x80);			// set RSSIMODE1 to initiate RSSI measurement

	RSSIcheck = read_short (READ_BBREG6);		// Read RSSIRDY
	while ((RSSIcheck & 0x01) != 0x01)		// Wait until RSSIRDY goes to 1; this indicates result is ready
		RSSIcheck = read_short (READ_BBREG6);	// this takes max 8 symbol periods (16 uS each = 128 uS)

	RSSIcheck = read_long(0x210);			// read the RSSI

	write_short(WRITE_BBREG6, 0x40);			// enable RSSI on received packets again after energy scan is finished

	#if defined(ENABLE_PA_LNA)
		write_short(WRITE_GPIO, 0);
		write_short(WRITE_GPIODIR, 0x00);		// Set GPIO direction to INPUT
		write_long(TESTMODE, 0x0F);			// setup for automatic PA/LNA control
	#endif

	return RSSIcheck;
}

// TX side - what goes in the TX FIFO (MRF24J40 datahseet figure 3-11):
//
// Size Offset	Descr
// 1		0		Header length (m)
// 1		1		Frame length (m+n)
// 1		2		LSB of Frame Control (bits/i)
// 1		3		MSB of Frame Control (type)
// 1		4		Sequence number
// 20		24		Addressing fields, worst case (PANIDx2 = 4, LONGx2=16 total =20)
// 103		127		Payload (from TxBuffer)

// sends raw packet per already setup Tx structure.  No error checking here.
void Radio::TXRaw(void)
{
	uint8_t wReg;													// radio write register (into TX FIFO starting at long addr 0)

	wReg = toTXfifo(2,BytePtr(Tx)+1,2+1);						// frame control (2) + sequence number (1) 

	if (Tx.dstAddrMode == SHORT_ADDR_FIELD)						// if a short dest addr is present
	{
		wReg = toTXfifo(wReg,BytePtr(Tx.dstPANID), 2);			// write dstPANID
		wReg = toTXfifo(wReg,BytePtr(Tx.dstAddr), 2);			// write short address
	}
	else if (Tx.dstAddrMode == LONG_ADDR_FIELD)					// if a long dest addr is present
	{
		wReg = toTXfifo(wReg,BytePtr(Tx.dstPANID), 2);			// write dstPANID
		wReg = toTXfifo(wReg,BytePtr(Tx.dstAddr), 8);			// long addr
	}

	// now wReg is at start of source PANID (if present)

	if ( Tx.srcAddrMode != NO_ADDR_FIELD && 					// if source present
		 Tx.dstAddrMode != NO_ADDR_FIELD && 					// and dest present
		 !Tx.panIDcomp )										// and no PANID compression
			wReg = toTXfifo(wReg,BytePtr(Tx.srcPANID), 2);		// then write src PANID
		
	if (Tx.srcAddrMode == SHORT_ADDR_FIELD)						// if a short src addr is present
		wReg = toTXfifo(wReg,BytePtr(Tx.srcAddr), 2);
	else if (Tx.srcAddrMode == LONG_ADDR_FIELD)					// if a long src addr is present
		wReg = toTXfifo(wReg,BytePtr(Tx.srcAddr), 8);
	
	// now wReg is pointing to first wReg after header (m)
	
	write_long(0, wReg-2);										// header length, m (-2 for header & frame lengths)

	wReg = toTXfifo(wReg,Tx.payload,Tx.payloadLength);

	write_long(1, wReg-2);										// frame length (m+n)

	RadioStatus.TX_BUSY = 1;									// mark TX as busy TXing
	RadioStatus.TX_PENDING_ACK = Tx.ackRequest;

	write_short(WRITE_TXNMTRIG, Tx.ackRequest << 2 | 1);			// kick off transmit with above parameters
	RadioStatus.LastTXTriggerTick = ReadCoreTimer();			// record time (used to check for locked-up radio or PLL loss)
}

// Sends next packet from Tx.  Blocks for up to MRF24J40_TIMEOUT_TICKS if transmitter is
// not ready (RadioStatus.TX_BUSY).  If you don't want to be blocked, don't call
// ths until RadioStatus.TX_BUSY == 0.  
//
// This automatically sets frame number and source address for you
void Radio::TXPacket(void)
{
	if (Tx.srcAddrMode == SHORT_ADDR_FIELD)
		Tx.srcAddr = RadioStatus.MyShortAddress;
	else if (Tx.srcAddrMode == LONG_ADDR_FIELD)
		Tx.srcAddr = RadioStatus.MyLongAddress;

	Tx.frameNumber = RadioStatus.IEEESeqNum++;

	while(RadioStatus.TX_BUSY)									// If TX is busy, wait for it to clear (for a resaonable time)
		if ( CtTicksSince(RadioStatus.LastTXTriggerTick) > MRF24J40_TIMEOUT_TICKS )	// if not ready in a resonable time
			initMrf24j40();										// reset radio hardware (stay on same channel)

	TXRaw();
}


// returns status of last transmitted packet: TX_SUCCESS (1), TX_FAILED (2), or 0 = no result yet because TX busy
uint8_t Radio::TXResult(void)
{
	if (RadioStatus.TX_BUSY)									// if TX not done yet
		return TX_RESULT_BUSY;
				
	return TX_RESULT_SUCCESS + RadioStatus.TX_FAIL;				// 1=success, 2=fail
}

// returns TX_RESULT_SUCCESS or TX_RESULT_FAILED.  Waits up to MRF24J40_TIMEOUT_TICKS.
uint8_t Radio::WaitTXResult(void)
{
	while(RadioStatus.TX_BUSY)									// If TX is busy, wait for it to clear (for a resaonable time)
		if ( CtTicksSince(RadioStatus.LastTXTriggerTick) > MRF24J40_TIMEOUT_TICKS )		// if not ready in a resonable time
			initMrf24j40();										// reset radio hardware (stay on same channel)

	return TX_RESULT_SUCCESS + RadioStatus.TX_FAIL;				// 1=success, 2=fail
}


//	RX side - what goes in RXBuffer (from MRF24J40 datasheet figure 3-9)
//
//	Size	Offset
//	1		0		Frame length (m+n+2 = header + 102 + FCS)
//	1		1		LSB of Frame Control (bits)
//	1		2		MSB of Frame Control (type)
//	1		3		Sequence number
//	20		23		Addressing fields, worst case (PANIDx2 = 4, MACx2=16 total =20)
//	103		126		Payload
//	2		128		FCS
//	1		129		LQI
//	1		130		RSSI

// Returns count of received packets waiting to be processed & discarded.  Next packet to process is in "Rx".
// Note this gives you ALL received packets (not just ones addressed to you).   Check the addressing yourself if you care.
// Also be aware that sucessive identical packets (same frame number) will be received if the far-end misses your ACK (it
// will re-transmit).  Check for that if you care.
uint8_t Radio::RXPacket(void)
{
	if (!RadioStatus.RXPacketCount)
		return 0;														// no packets to process

	uint8_t* readPoint = (uint8_t*)RXBuffer[RadioStatus.RXReadBuffer];		// recieved packet read point

	if(RadioStatus.TX_BUSY)												// time out and reset radio if we missed interrupts for a long time
		if ( CtTicksSince(RadioStatus.LastTXTriggerTick) > MRF24J40_TIMEOUT_TICKS )
				initMrf24j40();											// reset radio hardware (stay on same channel)

	readPoint = readBytes(BytePtr(Rx), readPoint, 1+2+1+2);				// copy frame length (1), frame control (2), frame number (1), PANID (2)
	
	if( Rx.securityEnabled )											// if security enabled, toss it (not supported)
	{
		RadioStatus.RXSecurityEnabled++;								// log error
		RadioDiscardPacket();
		return RadioRXPacket();											// yes I know it's a little recursive, but the RXBuffer is small enough that the stack is unlikely to overflow
	}

	if (Rx.frameType == PACKET_TYPE_ACK)								// no PANID present on ACK frames [802.15.4 weakness: No way to know if this ACK is really for you]
		readPoint -= 2;

	// readPoint now just after first PANID field

	if (Rx.dstAddrMode == SHORT_ADDR_FIELD)								// if a short dest addr is present
		readPoint = readBytes(BytePtr(Rx.dstAddr), readPoint, 2);
	else if (Rx.dstAddrMode == LONG_ADDR_FIELD)							// if a long dest addr is present
		readPoint = readBytes(BytePtr(Rx.dstAddr), readPoint, 8);

	Rx.srcPANID = Rx.dstPANID;											// copy first PANID because we don't know if it's src or dst yet
	Rx.srcAddr = Rx.dstAddr;											// ditto for address

	// now readPoint is at start of source PANID (if present)

	if ( Rx.srcAddrMode != NO_ADDR_FIELD && 							// if source present
		 Rx.dstAddrMode != NO_ADDR_FIELD && 							// and dest present
		 !Rx.panIDcomp )												// and no PANID compression
			readPoint = readBytes(BytePtr(Rx.srcPANID),readPoint, 2);	// then read src PANID
		
	if (Rx.srcAddrMode == SHORT_ADDR_FIELD)								// if a short src addr is present
		readPoint = readBytes(BytePtr(Rx.srcAddr),readPoint, 2);
	else if (Rx.srcAddrMode == LONG_ADDR_FIELD)							// if a long src addr is present
		readPoint = readBytes(BytePtr(Rx.srcAddr),readPoint,8);
	
	Rx.payload = readPoint;												// now readPoint points at the start of the payload
	Rx.payloadLength = Rx.frameLength - (readPoint - RXBuffer[RadioStatus.RXReadBuffer]) + 1;

	Rx.lqi = RXBuffer[RadioStatus.RXReadBuffer][RXBuffer[RadioStatus.RXReadBuffer][0]+3];
	Rx.rssi = RXBuffer[RadioStatus.RXReadBuffer][RXBuffer[RadioStatus.RXReadBuffer][0]+4];

	return RadioStatus.RXPacketCount;
}

void Radio::DiscardPacket(void)
{
	if (RadioStatus.RXPacketCount)										// just in case we get called more than we ought
	{
		RadioStatus.RXPacketCount--;
		RadioStatus.RXReadBuffer = (RadioStatus.RXReadBuffer + 1) & (PACKET_BUFFERS - 1);
	}
	else
		RadioStatus.RadioExtraDiscard++;
}


bool Radio::RadioSetAddress( uint16_t MyShortAddress,uint64_t MyLongAddress, uint16_t MyPANID){
		return true;
	}



	template <typename T>
	T Radio::BytePtr( T & valor){
		T tmp ;
	return static_cast<T>(valor>>(8*tmp++));
	}









uint32_t Radio::CtTicksSince(uint32_t value){

return value;
}







// Interrupt handler for the MRF24J40 and P2P stack (PIC32 only, no security)
void isr()//__ISR(_EXTERNAL_4_VECTOR, ipl4) _INT4Interrupt(void)				// from INT pin on MRF24J40 radio
{/*
	MRF24J40_IFREG iflags;

	PUSH_DEBUG_STATE();
	SET_DEBUG_STATE(CPU_BUSY);

	RFIF = 0;															// clear IF immediately to allow next interrupt

	iflags.Val = read_short(READ_ISRSTS);									// read ISR to see what caused the interrupt

	if(iflags.bits.RXIF)												// RX int?
	{
		uint8_t i, bytes;

		write_short(WRITE_BBREG1, 0x04);									// set RXDECINV to disable hw RX while we're reading the FIFO

		bytes = read_long(0x300) + 2;									// get the size of the packet w/FCS, + 2 more bytes for RSSI and LQI

		if( bytes > RX_BUFFER_SIZE)										// if too big for the RX buffer
		{
			RadioStatus.RXPacketTooBig++;
			bytes = RX_BUFFER_SIZE;										// truncate to fit
		}

		RXBuffer[RadioStatus.RXWriteBuffer][0] = bytes - 4;				// store length of packet (not counting length byte, FCS, LQI and RSSI)

		for(i=1;i<=bytes;i++)											// copy data from the FIFO into the RX buffer, plus RSSI and LQI
			RXBuffer[RadioStatus.RXWriteBuffer][i] = read_long(0x300+i);

		RadioStatus.RXPacketCount++;
		RadioStatus.RXWriteBuffer = (RadioStatus.RXWriteBuffer+1) & (PACKET_BUFFERS-1);	// mod PACKET_BUFFERS

		if ( (RadioStatus.RXPacketCount > PACKET_BUFFERS) || (RadioStatus.RXWriteBuffer == RadioStatus.RXReadBuffer) )
			RadioStatus.RXBufferOverruns++;

		write_short(WRITE_RXFLUSH,0x01);									// flush RX hw FIFO manually (workaround for silicon errata #1)
		write_short(WRITE_BBREG1, 0x00);									// reset RXDECINV to enable radio to receive next packet
	}

	if(iflags.bits.TXIF)												// TX int?  If so, this means TX is no longer busy, and the result (if any) of the ACK request is in
	{
		RadioStatus.TX_BUSY = 0;										// clear busy flag (TX is complete now)

		if(RadioStatus.TX_PENDING_ACK)									// if we were waiting for an ACK
		{
			uint8_t TXSTAT = read_short(READ_TXSR);							// read TXSTAT, transmit status register
			RadioStatus.TX_FAIL    = TXSTAT & 1;						// read TXNSTAT (TX failure status)
			RadioStatus.TX_RETRIES = TXSTAT >> 6;						// read TXNRETRY, number of retries of last sent packet (0..3)
			RadioStatus.TX_CCAFAIL = TXSTAT & 0b00100000;				// read CCAFAIL

			RadioStatus.TX_PENDING_ACK = 0;								// TX finished, clear that I am pending an ACK, already got it (if I was gonna get it)
		}
	}


	// #ifdef SPI_INTERRUPTS
	// 		if (SPI2STATbits.SPITBE) 
	// 			INTSetFlag(INT_SPI2TX);			
	// #endif


	// POP_DEBUG_STATE();*/
}

void RadioInitP2P(void)
{
	Tx.frameType = PACKET_TYPE_DATA;
	Tx.securityEnabled = 0;
	Tx.framePending = 0;
	Tx.ackRequest = 1;
	Tx.panIDcomp = 1;
	Tx.dstAddrMode = SHORT_ADDR_FIELD;
	Tx.frameVersion = 0;
	Tx.srcAddrMode = NO_ADDR_FIELD;
	Tx.dstPANID = RadioStatus.MyPANID;
	Tx.dstAddr = RadioStatus.MyShortAddress;
	Tx.payload = 0x00;//txPayload;//es un puntero
}


}//end name space MRF24