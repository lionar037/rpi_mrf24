#pragma once
#include <iostream>

#include <display/include/oled.h>
//#include <thread>
//#include <vector>
#include <memory>

//verifica si es de 32 o 64 bits
//#if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4))

namespace RUN{
    struct Run_t{
        //Run_t() {std::cout<<"Runt_t() \n";}//=default;

        Run_t(){
        auto  oled = std::make_unique<OLED::Oled_t>(128, 64, BCM2835_I2C_CLOCK_DIVIDER_626, 0x3C);
        
                if (!oled->begin()) {
                    return -1;
                }        
                oled->clearScreen();
                oled->displayText("MP3 to Bluetooth ", 0, 0);
        }
        ~Run_t(){std::cout<<"~Runt_t() \n";}//=default;
        int start();
    };
}
