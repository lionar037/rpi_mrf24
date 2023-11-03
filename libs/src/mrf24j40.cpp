#include <cmd_mrf24j40.h>
#include <mrf24j40.h>
#include <tyme.h>
#include <dbg.h>


namespace MRF24J40{

            // aMaxPHYPacketSize = 127, from the 802.15.4-2006 standard.
    static uint8_t rx_buf[127];

    static int ignoreBytes { 0 }; // bytes to ignore, some modules behaviour.
    static bool bufPHY { false }; // flag to buffer all bytes in PHY Payload, or not
    static rx_info_t rx_info{};
    static tx_info_t tx_info{};

    Mrf24j::Mrf24j()
    : prt_spi {std::make_unique<SPI::Spi>()}
    {
        #ifdef DBG
            std::cout <<"Mrf24j( )\r\n";
        #endif
    }

    const uint8_t Mrf24j::read_short(uint8_t address) {
            // 0 top for short addressing, 0 bottom for read
        const uint8_t tmp = (address<<1 & 0b01111110);
        const uint8_t ret = prt_spi->Transfer2bytes(tmp); // envia 16 , los mas significativos en 0x00 , los menos significativos envia el comando
//printf("2:0x%x ",ret);
        return ret;
    }

    void Mrf24j::write_short(uint8_t address, uint8_t data) {
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

    void Mrf24j::set_pan(uint16_t panid) {
        write_short(MRF_PANIDH, (panid >> 8)& 0xff);
        write_short(MRF_PANIDL, panid & 0xff);
    }

    void Mrf24j::address16_write(uint16_t address16) {
        write_short(MRF_SADRH, (address16 >> 8)& 0xff);
        write_short(MRF_SADRL, address16 & 0xff);
    }

    uint16_t Mrf24j::address16_read(void) {
        const uint8_t a16h = read_short(MRF_SADRH);
        return (a16h << 8 | read_short(MRF_SADRL));
    }

        /**
         * Simple send 16, with acks, not much of anything.. assumes src16 and local pan only.
         //* @param data
        */

    void Mrf24j::send16(uint16_t dest16, const char * data) {
        const uint8_t len = strlen(data); // get the length of the char* array
        int i = 0;
        write_long(i++, bytes_MHR); // header length
                        // +ignoreBytes is because some module seems to ignore 2 bytes after the header?!.
                        // default: ignoreBytes = 0;
        write_long(i++, bytes_MHR+ignoreBytes+len);

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
        write_long(i++, dest16 & 0xff);  // dest16 low
        write_long(i++, dest16 >> 8); // dest16 high

        const uint16_t src16 = address16_read();
        write_long(i++, src16 & 0xff); // src16 low
        write_long(i++, src16 >> 8); // src16 high

                // All testing seems to indicate that the next two bytes are ignored.
                //2 bytes on FCS appended by TXMAC
        i+=ignoreBytes;
        for (int q = 0; q < len; q++) {
            write_long(i++,data[q]);
        }
        // ack on, and go!
        write_short(MRF_TXNCON, (1<<MRF_TXNACKREQ | 1<<MRF_TXNTRIG));
    }

    void Mrf24j::set_interrupts(void) {
            // interrupts for rx and tx normal complete
        write_short(MRF_INTCON, 0b11110110);
    }

            /** use the 802.15.4 channel numbers..
            */
    void Mrf24j::set_channel(uint8_t channel) {
        write_long(MRF_RFCON0, (((channel - 11) << 4) | 0x03));
    }

    void Mrf24j::init(void) {
            /*
            // Seems a bit ridiculous when I use reset pin anyway
            write_short(MRF_SOFTRST, 0x7); // from manual
            while (read_short(MRF_SOFTRST) & 0x7 != 0) {
                ; // wait for soft reset to finish
            }*/
            
           delay(192); 
        write_short(MRF_PACON2, 0x98);  // – Initialize FIFOEN = 1 and TXONTS = 0x6.
        write_short(MRF_TXSTBL, 0x95);  // – Initialize RFSTBL = 0x9.

        write_long(MRF_RFCON0, 0x03);   // – Initialize RFOPT = 0x03.
        write_long(MRF_RFCON1, 0x01);   // – Initialize VCOOPT = 0x02.
        write_long(MRF_RFCON2, 0x80);   // – Enable PLL (PLLEN = 1).
        write_long(MRF_RFCON6, 0x90);   // – Initialize TXFIL = 1 and 20MRECVR = 1.
        write_long(MRF_RFCON7, 0x80);   // – Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator).
        write_long(MRF_RFCON8, 0x10);   // – Initialize RFVCO = 1.
        write_long(MRF_SLPCON1, 0x21);  // – Initialize CLKOUTEN = 1 and SLPCLKDIV = 0x01.

            //  Configuration for nonbeacon-enabled devices (see Section 3.8 “Beacon-Enabled and
            //  Nonbeacon-Enabled Networks”):
        write_short(MRF_BBREG2, 0x80);      // Set CCA mode to ED
        write_short(MRF_CCAEDTH, 0x60);     // – Set CCA ED threshold.
        write_short(MRF_BBREG6, 0x40);      // – Set appended RSSI value to RXFIFO.
        set_interrupts();
        set_channel(24);                    //original 12
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
        const uint8_t last_interrupt = read_short(MRF_INTSTAT);
        if (last_interrupt & MRF_I_RXIF) {
            flag_got_rx++;
                // read out the packet data...
            noInterrupts();
            rx_disable();
                // read start of rxfifo for, has 2 bytes more added by FCS. frame_length = m + n + 2
            const uint8_t frame_length = read_long(0x300);

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
            uint8_t rx_tmp[128];
            for (int i = 0; i < rx_datalength(); i++) {
                rx_tmp[i]=rx_info.rx_data[rd_ptr++] = read_long(0x301 + bytes_MHR + i);
            }

 printf(" buffer :\n");
            for (int i = 0; i < rx_datalength(); i++) {
               printf(" 0x%x\n" , rx_tmp[i]);
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
            flag_got_tx++;
            uint8_t tmp = read_short(MRF_TXSTAT);
                // 1 means it failed, we want 1 to mean it worked.
            tx_info.tx_ok = !(tmp & ~(1 << TXNSTAT));
            tx_info.retries = tmp >> 6;
            tx_info.channel_busy = (tmp & (1 << CCAFAIL));
        }
    }

            /**
             * Call this function periodically, it will invoke your nominated handlers
             */
    void Mrf24j::check_flags(void (*rx_handler)(), void (*tx_handler)()){
            // TODO - we could check whether the flags are > 1 here, indicating data was lost?
        if (flag_got_rx) {
            flag_got_rx = 0;
            rx_handler();
        }
        if (flag_got_tx) {
            flag_got_tx = 0;
            tx_handler();
        }
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
        return rx_info.frame_length - bytes_nodata;
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

    void Mrf24j::delay(uint16_t t){
        TYME::Time_t time ;
        time.delay_ms(t);
    return;
    }

    void Mrf24j::interrupts(){
        
    }

    void Mrf24j::noInterrupts(){
    }

    // void Mrf24j::reset(void) {

    // }

    Mrf24j::~Mrf24j( ){
        #ifdef DBG
            std::cout <<"~Mrf24j( )\r\n";
        #endif
    }

}
