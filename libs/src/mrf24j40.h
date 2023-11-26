#pragma once
    #include <iostream>
    #include <memory>
    #include <cstring>
    #include <spi.h>
    #include <config.h>


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
    uint8_t         tx_ok           :1;
    uint8_t         retries         :2;
    uint8_t         channel_busy    :1;
} tx_info_t;

struct Mrf24j : public SPI::Spi
{
    public:
        Mrf24j( );
        ~Mrf24j( );

       // void reset(void);
        void                init                (void);

        const uint8_t       read_short          (uint8_t);            //address
        const uint8_t       read_long           (uint16_t);            //address
        void                write_short         (uint8_t , uint8_t );   //address ,data
        void                write_long          (const uint16_t , const uint8_t);//address ,data
        uint16_t            get_pan             (void);
        void                set_pan             (uint16_t);                 //panid
        void                address16_write     (uint16_t);         //address16
        void                address64_write     (uint64_t);
        uint16_t            address16_read      (void);
        uint64_t            address64_read      (void);
        void                set_interrupts      (void);

                //void set_promiscuous(__OBJC_BOOL_IS_BOOL );
        void                set_promiscuous     (bool );  
                /**
                 * Set the channel, using 802.15.4 channel numbers (11..26)
                 */
        void                set_channel         (uint8_t);
        void                rx_enable           (void);
        void                rx_disable          (void);
                                   /**IMPLEMENTADO  */

        void                pinMode             (int,bool);
        void                digitalWrite        (int,bool);
        void                delay               (uint16_t);
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
        void                 send16(uint16_t ,const char *);
        void                 send64(uint64_t ,const char *);
        void                 interrupt_handler(void);
        void                 check_flags(void (*rx_handler)(), void (*tx_handler)());
        void                 init_mrf(void);

    private:
        std::unique_ptr<SPI::Spi> prt_spi {};

            // essential for obtaining the data frame only
            // bytes_MHR = 2 Frame control + 1 sequence number + 2 panid + 2 shortAddr Destination + 2 shortAddr Source
            const int bytes_MHR {9};
            const int bytes_FCS {2}; // FCS length = 2
            const int bytes_nodata { }; // no_data bytes in PHY payload,  header length + FCS

            volatile uint8_t flag_got_rx{};
            volatile uint8_t flag_got_tx{};
    };
}

