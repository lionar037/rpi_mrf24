#pragma once

#include <app/work.h>
#define DELAY 150

#define myOLEDwidth  128
#define myOLEDheight 64

namespace OLED{

    struct Oled_t : public WORK::Work_t
    {
        explicit Oled_t();
        ~Oled_t ()=default;
        bool create(const char*)override;
        bool init();
        void End();
        void Setup(); 
        void Start();
        private :
        const uint16_t I2C_Speed    { 626 }; //  bcm2835I2CClockDivider 
        const uint8_t I2C_Address   { 0x3C };
    };

}