#pragma once
#include <app/work.h>
#define DELAY 150

#define myOLEDwidth  128
#define myOLEDheight 64

namespace OLED{

    struct Oled_t : public WORK::Work_t
    {
        Oled_t  ()=default;
        ~Oled_t ()=default;
        bool create(const char*)override;
        bool init();
        void EndTest();
        void SetupTest(); 
        void TestLoop();
        private :
        const uint16_t I2C_Speed    { 626 }; //  bcm2835I2CClockDivider 
        const uint8_t I2C_Address   { 0x3C };
    };

}