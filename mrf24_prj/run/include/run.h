#pragma once
#include <iostream>

#include <display/include/oled.h>
#include <others/include/tyme.h>
//#include <thread>
//#include <vector>
#include <memory>


//verifica si es de 32 o 64 bits
//#if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4))

namespace RUN{
    struct Run_t{

        Run_t(){
            #ifdef DBG
                std::cout<<"Runt_t::Runt_t() \n";
            #endif
        }

        ~Run_t(){
            #ifdef DBG
            std::cout<<"~Runt_t() \n";
            #endif
        }
        int start();
    };
}
