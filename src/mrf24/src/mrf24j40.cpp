#include <mrf24/src/mrf24j40_cmd.h>
#include <mrf24/src/mrf24j40_settings.h>
#include <mrf24/src/mrf24j40_control_register.h>
#include <others/src/tyme.h>
#include <app/src/config.h>
#include <app/src/data_analisis.h>
#include <mrf24/src/mrf24j40.h>
#include <spi/src/spi.h>


namespace MRF24J40{
            // aMaxPHYPacketSize = 127, from the 802.15.4-2006 standard.
    static uint8_t rx_buf[127];

    static int ignoreBytes { 0 }; // bytes to ignore, some modules behaviour.
    static bool bufPHY { false }; // flag to buffer all bytes in PHY Payload, or not
    static rx_info_t rx_info{};
    static tx_info_t tx_info{};
    static RXMCR rxmcr{0x00};

    Mrf24j::Mrf24j()
    : prt_spi {std::make_unique<SPI::Spi>()} , m_bytes_nodata { m_bytes_MHR + m_bytes_FCS}
    {
        #ifdef DBG
            std::cout <<"Mrf24j( )\r\n";
        #endif
    }

    const uint8_t Mrf24j::read_short(const uint8_t address) {
            // 0 top for short addressing, 0 bottom for read
        const uint8_t tmp = (address<<1 & 0b01111110);
        const uint8_t ret = prt_spi->Transfer2bytes(tmp); // envia 16 , los mas significativos en 0x00 , los menos significativos envia el comando
//printf("2:0x%x ",ret);
        return ret;
    }

    void Mrf24j::write_short(const uint8_t address,const uint8_t data) {
            // 0 for top short address, 1 bottom for write
    const uint16_t lsb_tmp = ( (address<<1 & 0b01111110) | 0x01 ) | (data<<8);
        prt_spi->Transfer2bytes(lsb_tmp);
        return;
    }

    const uint8_t Mrf24j::read_long(const uint16_t address) {

        const uint8_t lsb_address = (address >> 3 )& 0x7F;//0x7f
        const uint8_t msb_address = (address << 5) & 0xE0;//0xe0

        const uint32_t tmp = ( (0x80 | lsb_address) | (msb_address <<8) ) &  0x0000ffff;
	    const uint8_t ret = prt_spi->Transfer3bytes(tmp);
        
    return ret;
    }

    void Mrf24j::write_long(const uint16_t address,const uint8_t data) {
        const uint8_t lsb_address = (address >> 3) & 0x7F;
        const uint8_t msb_address = (address << 5) & 0xE0;
        const uint32_t comp = ( (0x80 | lsb_address) | ( (msb_address | 0x10) << 8 ) | (data<<16) ) & 0xffffff;
        prt_spi->Transfer3bytes(comp);
        return;
    }

    uint16_t Mrf24j::get_pan(void) {
        const uint8_t panh = read_short(MRF_PANIDH);
        return (panh << 8 | read_short(MRF_PANIDL));
    }

    void Mrf24j::set_pan(const uint16_t panid) {
        write_short(MRF_PANIDH, (panid >> 8)& 0xff);
        write_short(MRF_PANIDL, panid & 0xff);
    }

    void Mrf24j::address16_write(const uint16_t address16) {
        write_short(MRF_SADRH, (address16 >> 8)& 0xff);
        write_short(MRF_SADRL, address16 & 0xff);
    }

    void Mrf24j::address64_write(const uint64_t addressLong){
        write_short(MRF_EADR7,(addressLong>>56)&0xff);
        write_short(MRF_EADR6,(addressLong>>48)&0xff);
        write_short(MRF_EADR5,(addressLong>>40)&0xff);
        write_short(MRF_EADR4,(addressLong>>32)&0xff);
        write_short(MRF_EADR3,(addressLong>>24)&0xff);
        write_short(MRF_EADR2,(addressLong>>16)&0xff);
        write_short(MRF_EADR1,(addressLong>>8 )&0xff);
        write_short(MRF_EADR0,(addressLong)&0xff);
    return ;
    }

    void Mrf24j::address_write(const uint64_t addressLong)
    {

        write_short(MRF_EADR1,(addressLong>>8 )&0xff);
        write_short(MRF_EADR0,(addressLong)&0xff);

        if(addressLong>0xffff){
        write_short(MRF_EADR7,(addressLong>>56)&0xff);
        write_short(MRF_EADR6,(addressLong>>48)&0xff);
        write_short(MRF_EADR5,(addressLong>>40)&0xff);
        write_short(MRF_EADR4,(addressLong>>32)&0xff);
        write_short(MRF_EADR3,(addressLong>>24)&0xff);
        write_short(MRF_EADR2,(addressLong>>16)&0xff);
        }
    return ;
    }

    uint16_t Mrf24j::address16_read(void) {
        const uint8_t a16h = read_short(MRF_SADRH);
        return (a16h << 8 | read_short(MRF_SADRL));
    }


    uint64_t Mrf24j::address64_read(void){
        uint64_t address64 ;

        address64  = (read_short(MRF_EADR0));
        address64 |= (read_short(MRF_EADR1))<< 8;
        address64 |= static_cast<uint64_t>(read_short(MRF_EADR2))<<16;
        address64 |= static_cast<uint64_t>(read_short(MRF_EADR3))<<24;
        address64 |= static_cast<uint64_t>(read_short(MRF_EADR4))<<32;
        address64 |= static_cast<uint64_t>(read_short(MRF_EADR5))<<40;
        address64 |= static_cast<uint64_t>(read_short(MRF_EADR6))<<48;
        address64 |= static_cast<uint64_t>(read_short(MRF_EADR7))<<56;

    return  address64;
    }

    /**
     * Simple send 16, with acks, not much of anything.. assumes src16 and local pan only.
        * @param data
    */

    void Mrf24j::set_interrupts(void) {
            // interrupts for rx and tx normal complete
        write_short(MRF_INTCON, 0b11110110);
    }

            /** use the 802.15.4 channel numbers..
            */
    void Mrf24j::set_channel(const uint8_t channel) {
        write_long(MRF_RFCON0, (((channel - 11) << 4) | 0x03));
    }

    void Mrf24j::init(void) {
    // //Seems a bit ridiculous when I use reset pin anyway
    // write_short(MRF_SOFTRST, 0x7); // from manual
    // while (read_short(MRF_SOFTRST) & 0x7 != 0) {
        // ; // wait for soft reset to finish
    // }

           #include <app/src/config.h>
           #ifdef MODULE_TX_RST
            write_short(MRF_SOFTRST, 0x7); 
            #else
            //write_short(MRF_SOFTRST, 0x7); 
           #endif
                      delay(192); 
        write_short(MRF_PACON2, 0x98);  // – Initialize FIFOEN = 1 and TXONTS = 0x6.
        write_short(MRF_TXSTBL, 0x95);  // – Initialize RFSTBL = 0x9.

        write_long(MRF_RFCON0, 0x03);   // – Initialize RFOPT = 0x03.
        write_long(MRF_RFCON1, 0x01);   // – Initialize VCOOPT = 0x02.
        write_long(MRF_RFCON2, 0x80);   // – Enable PLL (PLLEN = 1).
        write_long(MRF_RFCON6, 0x90);   // – Initialize TXFIL = 1 and 20MRECVR = 1.
        write_long(MRF_RFCON7, 0x80);   // – Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator).
        write_long(MRF_RFCON8, 0x10);   // – Initialize RFVCO = 1.
        write_long(MRF_SLPCON1,0x21);  // – Initialize CLKOUTEN = 1 and SLPCLKDIV = 0x01.

        //  Configuration for nonbeacon-enabled devices (see Section 3.8 “Beacon-Enabled and
        //  Nonbeacon-Enabled Networks”):
        write_short(MRF_BBREG2, 0x80);      // Set CCA mode to ED
        write_short(MRF_CCAEDTH, 0x60);     // – Set CCA ED threshold.
        write_short(MRF_BBREG6, 0x40);      // – Set appended RSSI value to RXFIFO.
        set_interrupts();
        set_channel(CHANNEL);                    //original 12
        // max power is by default.. just leave it...
        // Set transmitter power - See “REGISTER 2-62: RF CONTROL 3 REGISTER (ADDRESS: 0x203)”.
        write_short(MRF_RFCTL, 0x04);       //  – Reset RF state machine.
        write_short(MRF_RFCTL, 0x00);       // part 2
        delay(192);                           // delay at least 192usec
    }

    /**
     * Call this from within an interrupt handler connected to the MRFs output
     * interrupt pin.  It handles reading in any data from the module, and letting it
     * continue working.
     * Only the most recent data is ever kept.
     */
            
    void Mrf24j::interrupt_handler(void) {
        const auto last_interrupt = read_short(MRF_INTSTAT);
        if(last_interrupt & MRF_I_RXIF) {
            m_flag_got_rx++;
                // read out the packet data...
            noInterrupts();
            rx_disable();
                // read start of rxfifo for, has 2 bytes more added by FCS. frame_length = m + n + 2
            const auto frame_length = read_long(0x300);

                // buffer all bytes in PHY Payload
            if(bufPHY){
                int rb_ptr = 0;
                for (int i = 0; i < frame_length; i++) { // from 0x301 to (0x301 + frame_length -1)
                    rx_buf[rb_ptr++] = read_long(0x301 + i);
                }
            }

            // buffer data bytes
            int rd_ptr = 0;
            // from (0x301 + bytes_MHR) to (0x301 + frame_length - bytes_nodata - 1)

            // printf(" frame length : %d \n",frame_length);
            // printf(" rx datalength : %d \n",rx_datalength());

        for (uint8_t i = 0; i < frame_length ; i++) {//original
           // for (uint16_t i = 0; i < frame_length + rx_datalength(); i++) {//original
                rx_info.rx_data[rd_ptr++] = read_long(0x301 + m_bytes_MHR + i);
            }
            rx_info.frame_length = frame_length;
                    // same as datasheet 0x301 + (m + n + 2) <-- frame_length
            rx_info.lqi = read_long(0x301 + frame_length);
                    // same as datasheet 0x301 + (m + n + 3) <-- frame_length + 1
            rx_info.rssi = read_long(0x301 + frame_length + 1);

            rx_enable();
            interrupts();
        }

        if (last_interrupt & MRF_I_TXNIF) {
            m_flag_got_tx++;
            const auto tmp = read_short(MRF_TXSTAT);
                // 1 means it failed, we want 1 to mean it worked.
            tx_info.tx_ok = !(tmp & ~(1 << TXNSTAT));
            tx_info.retries = tmp >> 6;
            tx_info.channel_busy = (tmp & (1 << CCAFAIL));
        }
    }


int Mrf24j::getStatusInfoTx(void)
{
    return tx_info.tx_ok ;
}
    /**
     * Call this function periodically, it will invoke your nominated handlers
     */
    bool Mrf24j::check_flags(void (*rx_handler)(), void (*tx_handler)())    
    {
            // TODO - we could check whether the flags are > 1 here, indicating data was lost?
        if (m_flag_got_rx) {
            m_flag_got_rx = 0;
            #ifdef DBG_MRF
                std::cout<< "\t\trecibe packete \n";
            #endif
            rx_handler();
            return true;
        }
        if (m_flag_got_tx) {
            m_flag_got_tx = 0;
            #ifdef DBG_MRF
                std::cout<< "\t\ttransmite packete \n";
            #endif
            tx_handler();
            return false;
        }
        return false;
    }


    bool Mrf24j::check_ack(void (*tx_handler)())    
    {
    // TODO - we could check whether the flags are > 1 here, indicating data was lost?
        if (m_flag_got_tx) {
            m_flag_got_tx = 0;
            #ifdef DBG_MRF
                std::cout<< "\trecibe ACK OK \n";
            #endif
            
            return true;
        }else{
            #ifdef DBG_MRF
                std::cout<< "\tACK not response  \n";
            #endif        
        }

        return false;
    }
    /**
     * Set RX mode to promiscuous, or normal
     */
    void Mrf24j::set_promiscuous(bool enabled) {
        if (enabled) {
            write_short(MRF_RXMCR, 0x01);
        } else {
            write_short(MRF_RXMCR, 0x00);
        }
    }


void Mrf24j::settings_mrf(void){
        rxmcr.PANCOORD=true;
        rxmcr.COORD=false;
        //rxmcr.PROMI=true;       //1 = Receive all packet types with good CRC
        rxmcr.PROMI=false;   //0 = Discard packet when there is a MAC address mismatch, illegal frame type, dPAN/sPAN or MACshort address mismatch (default)
        
        rxmcr.NOACKRSP=false;    //1 = Disables automatic Acknowledgement response
                                //0 = Enables automatic Acknowledgement response. Acknowledgements are returned when they are requested (default).
        #ifdef DBG
            printf("*reinterpret_cast : 0x%x\n",*reinterpret_cast<uint8_t*>(&rxmcr));
        #endif
        write_short(MRF_RXMCR, *reinterpret_cast<uint8_t*>(&rxmcr));
        return;
    }

    rx_info_t * Mrf24j::get_rxinfo(void) {
        return &rx_info;
    }

    tx_info_t * Mrf24j::get_txinfo(void) {
        return &tx_info;
    }

    uint8_t * Mrf24j::get_rxbuf(void) {
        return rx_buf;
    }

    int Mrf24j::rx_datalength(void) {
        return rx_info.frame_length - m_bytes_nodata;
    }

    void Mrf24j::set_ignoreBytes(int ib) {
        // some modules behaviour
        ignoreBytes = ib;
    }

    /**
     * Set bufPHY flag to buffer all bytes in PHY Payload, or not
     */
    void Mrf24j::set_bufferPHY(bool bp) {
        bufPHY = bp;
    }


    bool Mrf24j::get_bufferPHY(void) {
        return bufPHY;
    }

    /**
     * Set PA/LNA external control
     */
    void Mrf24j::set_palna(bool enabled) {
        if (enabled) {
            write_long(MRF_TESTMODE, 0x07); // Enable PA/LNA on MRF24J40MB module.
        }else{
            write_long(MRF_TESTMODE, 0x00); // Disable PA/LNA on MRF24J40MB module.
        }
    }

    void Mrf24j::rx_flush(void) {
        write_short(MRF_RXFLUSH, 0x01);
    }

    void Mrf24j::rx_disable(void) {
        write_short(MRF_BBREG1, 0x04);  // RXDECINV - disable receiver
    }

    void Mrf24j::rx_enable(void) {
        write_short(MRF_BBREG1, 0x00);  // RXDECINV - enable receiver
    }


    void Mrf24j::pinMode(int i, bool b){
    return;
    }

    void Mrf24j::digitalWrite(int i, bool b){
    return;
    }

    void Mrf24j::delay(const uint16_t t){
        TYME::Time_t time ;
        time.delay_ms(t);
    return;
    }

    void Mrf24j::interrupts(){
        
    }

    void Mrf24j::noInterrupts(){
    }

    Mrf24j::~Mrf24j( ){
        #ifdef DBG_MRF
            std::cout <<"~Mrf24j( )\r\n";
        #endif
    }


    void Mrf24j::send(uint64_t dest, const std::string& pf) 
    {
        //const uint8_t len = strlen(data); // get the length of the char* array
        const auto len = pf.length();
        int i = 0;
        write_long(i++, m_bytes_MHR); // header length
                        // +ignoreBytes is because some module seems to ignore 2 bytes after the header?!.
                        // default: ignoreBytes = 0;
        write_long(i++, m_bytes_MHR+ignoreBytes+len);

                        // 0 | pan compression | ack | no security | no data pending | data frame[3 bits]
        write_long(i++, 0b01100001); // first byte of Frame Control
                        // 16 bit source, 802.15.4 (2003), 16 bit dest,
        write_long(i++, 0b10001000); // second byte of frame control
        write_long(i++, 1);  // sequence number 1

        const uint16_t panid = get_pan();
        #ifdef DBG
            printf("\npanid: 0x%X\n",panid);
        #endif

        write_long(i++, panid & 0xff);  // dest panid
        write_long(i++, panid >> 8);

        write_long(i++, dest & 0xff);  // dest16 low
        write_long(i++, dest >> 8); // dest16 high
uint64_t src ;
        //if(sizeof(dest)>2){
        if(dest>0xffff){            
            #ifdef DBG_MRF
                std::cout <<"es un mac de 64 bytes\n";
            #endif
        write_long(i++, (dest >> 16 ) & 0xff);
        write_long(i++, (dest >> 24 ) & 0xff);
        write_long(i++, (dest >> 32 ) & 0xff);
        write_long(i++, (dest >> 40 ) & 0xff);
        write_long(i++, (dest >> 48 ) & 0xff);
        write_long(i++, (dest >> 56 ) & 0xff);

        src = address64_read();

        }
        else{
            #ifdef DBG_MRF
            std::cout <<"es un mac de 16 bytes\n";
        
        src = address16_read();

            #endif
        }
        write_long(i++, src & 0xff); // src16 low
        write_long(i++, src >> 8); // src16 high
 

       //if(sizeof(src)>2)
       if(dest>0xffff)            
       {
            write_long(i++, (src >> 16 ) & 0xff); 
            write_long(i++, (src >> 24 ) & 0xff); 
            write_long(i++, (src >> 32 ) & 0xff); 
            write_long(i++, (src >> 40 ) & 0xff); 
            write_long(i++, (src >> 48 ) & 0xff); 
            write_long(i++, (src >> 56 ) & 0xff); 
        }
                // All testing seems to indicate that the next two bytes are ignored.
                //2 bytes on FCS appended by TXMAC
         i+=ignoreBytes;

        for(const auto& byte : pf) write_long(i++,static_cast<char>(byte));
        
        // ack on, and go!
        write_short(MRF_TXNCON, (1<<MRF_TXNACKREQ | 1<<MRF_TXNTRIG));
    }

void  Mrf24j::settingsSecurity(void){
    SECCR2 securityConfig{0x00};
    securityConfig.TXG1CIPHER=AES_CBC_MAC_64;
    securityConfig.TXG2CIPHER=AES_CBC_MAC_64;
    securityConfig.UPDEC=true;         //Upper Layer Security Decryption Mode bit
    securityConfig.UPENC=true;         //Upper Layer Security Encryption Mode bit
    return ;
}


}//END NAMESPACE MRF24
