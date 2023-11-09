// Include file for Microchip MRF24J40 802.15.4 radio hardware
// New parts (c) 2010-2012 nerdfever.com
// Originally based on Microchip MiWi DE v.3.1.3, 5/28/2010 (c) Microchip

#pragma once

#include <iostream>

#include "radio_cmd.h"
#include "mrf24j40.h"

// radio config 
#define MS_TO_CORE_TICKS
#define	MRF24J40_TIMEOUT_TICKS	MS_TO_CORE_TICKS(19)	// reset radio hardware if no TX_BUSY stays true this long - PLL lock lost or other serious problem

#define PACKET_BUFFERS (2)								// radio RX packet buffers.  Must be a power of 2.
//#define TURBO_MODE									// propriatary MCHP modulation; up to 625 Kbps (less range tho) 
//#define ENABLE_PA_LNA									// for range on MB board

// hw setup

#if defined(ENABLE_PA_LNA)
//	MB (high power) board has SiGe PA2423L, gain is nominally +22.5 dBm with 45% power-added efficiency
//	For use in USA, set output power set to be -1.9dB with 0x18 (FCC limit is 100 mW)
//	For use in Europe (and to be safe, elsewhere) set at -14.9dB with 0x70 (EU limit is 10 mW)
	#define TX_POWER			(0x18)		// use 0x18 for USA, 0x70 for Europe (and elsewhere, to be safe); uses SiGe PA2423L power amp
	#define FULL_CHANNEL_MAP    0x03FFF800	// 0b0000 0011 1111 1111 1111 1000 0000 0000 = bits 11 to 25 (for higher power MB module)
#else
	#define TX_POWER			(0x00)		// power level to be 0dBms
	#define FULL_CHANNEL_MAP	0x07FFF800	// 0b0000 0111 1111 1111 1111 1000 0000 0000 = bits 11 to 26 (for MA module)
#endif

#define RX_BUFFER_SIZE 	(144)				// 144 bytes per MRF24J40 data sheet figure 2-6
#define TX_FIFO_SIZE	(128)				// size of TX FIFO buffer in radio hardware
#define TX_PAYLOAD_SIZE (103)				// max app payload size, bytes
#define RX_PAYLOAD_SIZE (TX_PAYLOAD_SIZE)

// typedefs


namespace MRF24J40{
typedef union					// interrupt mask for radio
{
	uint8_t Val;
	struct
	{
		uint8_t TXIF 			:1; // transmission finished interrupt (TX no longer busy)
		uint8_t 				:2;
		uint8_t RXIF 			:1; // received a packet interrupt
		uint8_t SECIF 		:1;	// received a secured packet interrupt
		uint8_t 				:4;
	} bits;
} MRF24J40_IFREG;

typedef struct									// radio state
{
	uint8_t 	TX_BUSY			:1;					// transmitter is busy.  Set when TX triggered, reset by TX ISR.
	uint8_t 	TX_PENDING_ACK  :1; 				// we are currently waiting for an ack (don't know yet if we'll get one)
	uint8_t 	TX_FAIL         :1;					// last packet sent failed
	uint8_t 	TX_RETRIES		:2;					// number of tx retries of last sent packet
	uint8_t	    TX_CCAFAIL		:1;					// tells if last tx failed due to channel too busy
	uint8_t 	SLEEPING		:1;					// tells if radio is asleep now
	uint8_t 	volatile RXWriteBuffer;				// buffer number to write to in ISR (modified by ISR)
	uint8_t 	RXReadBuffer;						// buffer number to read from
	uint8_t	    volatile RXPacketCount;				// number of buffers waiting to be read (modified by ISR)
	uint8_t 	IEEESeqNum;							// tx packet sequence number (initial value not important)
	uint8_t 	Channel;							// current radio channel
	uint16_t 	MyShortAddress;						// short (2 byte) address
	uint64_t    MyLongAddress;						// long (8 byte, MAC) address
	uint16_t 	MyPANID;							// PANID (2 bytes)
	uint32_t	ResetCount;							// count of times hardware has been reset
	uint32_t	LastTXTriggerTick;					// tick at which we triggered the last TX packet to go out
	// error statistics
	uint16_t	RXSecurityEnabled;					// number of packets RX's with security bit set (had to discard)
	uint16_t	RadioExtraDiscard;					// number of times we attempted to discard a packet that wasn't there (error)
	uint16_t	RXPacketTooBig;						// number of times we truncated a RX'ed packet that didn't fit in buffer
	uint16_t	RXBufferOverruns;					// number of times the RX buffer overran (because we didn't empty it fast enough)
} MRF24J40_STATUS;

typedef struct							
{
	// note that the 1st 4 bytes of this structure are identical to the 1st 4 bytes of the RX FIFO contents

	uint8_t		frameLength;			// bytes (m+n, per 802.15.4)  Does not count itself, 2 bytes of FCS, 1 of LQI, or 1 of RSSI.
	unsigned	frameType		:3;		// per Table 79
	unsigned	securityEnabled	:1;		// per Figure 42
	unsigned	framePending 	:1;		// per Figure 42
	unsigned	ackRequest		:1;		// per Figure 42
	unsigned	panIDcomp		:1;		// per Figure 42
	unsigned					:3;		// reserved bits in 802.15.4
	unsigned	dstAddrMode		:2;		// per Table 80
	unsigned	frameVersion	:2;		// per Figure 42
	unsigned	srcAddrMode		:2;		// per Table 80
	uint8_t		frameNumber;			// packet sequence number

	// from here down is NOT (necessarily) identical to FIFO contents

	uint16_t		dstPANID;
	uint64_t		dstAddr;				// only 1st 2 bytes used if short addr
	uint16_t		srcPANID;
	uint64_t		srcAddr;				// only 1st 2 bytes used if short addr
	
	uint8_t		    payloadLength;			// length of payload field
	uint8_t *		payload;				// points at payload start
	uint8_t         lqi;					// LQI value for the received packet
	uint8_t         rssi;					// RSSI value for the received packet
} PACKET;


	// function prototypes
	struct Radio:public Mrf24j{
			bool 		initMrf24j40	(void);//agregado nuevo
			bool 		RadioSetAddress	(const uint16_t,const uint64_t,const uint16_t);//agregado nuevo
			uint8_t 	toTXfifo		(uint16_t,uint8_t*, uint8_t);
			unsigned char* readBytes	(unsigned char* , unsigned char* , unsigned int );


			bool   		Init			(void);
			void   		SetAddress		(const uint16_t,const uint64_t,const uint16_t );
			bool   		SetChannel		(const uint8_t);
			void   		SetSleep		(const uint8_t);
			uint8_t  	EnergyDetect	(void);
			void   		TXRaw			(void);
			void   		TXPacket		(void);
			uint8_t  	RXPacket		(void);
			uint8_t  	TXResult		(void);//uint8_t Radio::RadioTXResult(void)
			uint8_t  	WaitTXResult	(void);
			void   		DiscardPacket	(void);
			uint32_t 	ReadCoreTimer	(void);//agregado nuevo
			
			uint32_t CtTicksSince(uint32_t);

			template <typename T> 
			uint8_t* BytePtr( T&);

		private:
			//agregado nuevo
			bool RFIE{0};
	};

// public variables
}