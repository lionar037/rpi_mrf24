#pragma once
#include <iostream>
#include <cstdint>
#include <app/include/config.h>


namespace DEBUGGER{

    struct Dbg_t
    {
        Dbg_t()=default;
        ~Dbg_t()=default;
        void debug(){
            std::cout << " step1"<< std::to_string(++Next) <<"\n";
        }                
        uint16_t step{++Next};
        inline static uint16_t Next{0};
    };

        void debug(){
            #ifdef DBG
            static uint16_t Next{0};
            std::cout << " step : "<< std::to_string(++Next) <<"\n";
            #endif
        }     

}
