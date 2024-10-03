#pragma once
    #include <iostream>
    #include <memory>
    #include <cstring>

    #include <app/include/config.h>
    #include <spi/include/spi.h>
namespace DATA{
        struct packet_tx;
    }

namespace MRF24J40{

typedef struct _rx_info_t{
    uint8_t         frame_length         ;
    uint8_t         rx_data         [116]; //max data length = (127 aMaxPHYPacketSize - 2 Frame control - 1 sequence number - 2 panid - 2 shortAddr Destination - 2 shortAddr Source - 2 FCS)
    uint8_t         lqi                  ;
    uint8_t         rssi                 ;
} rx_info_t;

    /**
     * Based on the TXSTAT register, but "better"
     */
typedef struct _tx_info_t{
    uint8_t         tx_ok           :1; //  bit 0 TXNSTAT: TX Normal FIFO Release Status bit 
                                        //  1 = Failed, retry count exceeded
                                        //  0 = Succeeded
    uint8_t         retries         :2; //0 = Succeeded
    uint8_t         channel_busy    :1;
} tx_info_t;

struct Mrf24j : public SPI::Spi
{
    public:
        Mrf24j( );
        ~Mrf24j( );

       // void reset(void);
        void                init                (void);

        const uint8_t       read_short          (const uint8_t);            //address
        const uint8_t       read_long           (const uint16_t);            //address
        void                write_short         (const uint8_t ,const uint8_t );   //address ,data
        void                write_long          (const uint16_t , const uint8_t);//address ,data
        uint16_t            get_pan             (void);
        void                set_pan             (const uint16_t);                 //panid
        void                address16_write     (const uint16_t);         //address16
        void                address64_write     (const uint64_t);
        uint16_t            address16_read      (void);
        uint64_t            address64_read      (void);
        void                set_interrupts      (void);
        void                address_write       (const uint64_t);

                //void set_promiscuous(__OBJC_BOOL_IS_BOOL );
        void                set_promiscuous     (bool );  
                /**
                 * Set the channel, using 802.15.4 channel numbers (11..26)
                 */
        void                set_channel         (const uint8_t);
        void                rx_enable           (void);
        void                rx_disable          (void);
                                   /**IMPLEMENTADO  */

        void                pinMode             (int,bool);
        void                digitalWrite        (int,bool);
        void                delay               (const uint16_t);
        void                interrupts          (void);
        void                noInterrupts        (void);
        
                    /** If you want to throw away rx data */
        void                rx_flush(void);
        rx_info_t *         get_rxinfo(void) ;
        tx_info_t *         get_txinfo(void) ;
        uint8_t *           get_rxbuf(void) ;
        int                 rx_datalength(void);
        void                set_ignoreBytes(int );
                    /**
                     * Set bufPHY flag to buffer all bytes in PHY Payload, or not
                     */
        void                 set_bufferPHY(bool);
        bool                 get_bufferPHY(void);
                    /**
                    * Set PA/LNA external control
                    */
        void                 set_palna(bool);
       // void                 send16(uint16_t ,const char*);
        template <typename T>
        void                    send_template(uint64_t, const T&) ;

        void                    send(uint64_t ,const std::string& );
        //void                    send64(uint64_t ,const std::string&);
        void                    send64(uint64_t , const struct DATA::packet_tx&);
        void                    interrupt_handler(void);
        bool                    check_flags(void (*rx_handler)(), void (*tx_handler)());
        bool                    check_ack(void (*rx_handler)());    
        int                     getStatusInfoTx(void);    
        void                    settings_mrf(void);
        void                    settingsSecurity();
        void                    RadioSetSleep(uint8_t);
    private:
        std::unique_ptr<SPI::Spi> prt_spi {};

            // essential for obtaining the data frame only
            // bytes_MHR = 2 Frame control + 1 sequence number + 2 panid + 2 shortAddr Destination + 2 shortAddr Source
            const int m_bytes_MHR {9};
            const int m_bytes_FCS {2}; // FCS length = 2
            const int m_bytes_nodata { }; // no_data bytes in PHY payload,  header length + FCS

            volatile uint8_t m_flag_got_rx{};
            volatile uint8_t m_flag_got_tx{};
    };
}

