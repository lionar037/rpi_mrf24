#pragma once
#include <string_view>
#include <app/include/work.h>
#include <app/include/config.h>
#define DELAY 150

#define myOLEDwidth  128
#define myOLEDheight 64

namespace OLED{
    #ifdef USE_MRF24_RX
    struct Oled_t : public WORK::Work_t
    {
        explicit Oled_t();
        ~Oled_t();
            //bool create(const char*)override;
            bool            create(const std::string_view&)override;
            bool            init();
            void            End();
            void            Setup(); 
            void            Start();
            void            Graphics(const int,const int,const bool*,const uint8_t*);

        private :
            const uint16_t  I2C_Speed    { 626 }; //  bcm2835I2CClockDivider 
            const uint8_t   I2C_Address   { 0x3C };
            void            printOled(std::string_view );
            
    };
    #endif

}