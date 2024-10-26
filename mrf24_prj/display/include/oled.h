////////////////////////////////////////////////////////////////////////////                
//                
//
//                          oled.hpp
//
//
////////////////////////////////////////////////////////////////////////////

#pragma once

//#ifdef USE_MRF24_RX
        //#include <SSD1306_OLED.hpp>
#include <display/include/SSD1306_OLED.hpp>
//#endif
#include <cstdio>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
//#include <bcm2835.h>


#define FULLSCREEN (myOLEDwidth * (myOLEDheight / 8))


namespace OLED{
    class Oled_t {
    public:
        explicit Oled_t(const uint16_t width , const  uint16_t height , const  uint16_t i2c_speed , const  uint8_t i2c_address);
        ~Oled_t();
        void init(){}
        bool begin(bool i2c_debug = false);
        void clearScreen();
        void displayText(const char* text, int x, int y);
        void displayTextScroll(const char* text, int x, int y) ;
        void powerDown();
        void demo();
        void create(const std::string_view);
        //void Graphics(const int ,const int , const bool , uint8_t* );
        void Graphics(const int x,const int y,const bool* z,const uint8_t* w);

        const std::vector <std::string>& convertToMayuscule(std::vector <std::string>& display_text);
    
    protected:
        bool initI2C();
        void closeI2C();

    private:
        uint16_t myOLEDwidth;
        uint16_t myOLEDheight;                           
        uint16_t i2c_speed_;
        uint8_t i2c_address_;
        bool i2c_debug_;
        uint8_t* screenBuffer_;
        SSD1306 myOLED;
    };
}