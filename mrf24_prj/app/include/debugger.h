#pragma once
#include <iostream>
#include <cstdint>
#include <app/include/config.h>


namespace DEBUGGER{

uint16_t Next{0};

    struct Dbg_t
    {
        Dbg_t()=default;
        ~Dbg_t()=default;
        void debug(){
            std::cout << " step1"<< std::to_string(++m_Next) <<"\n";
        }                
        uint16_t step{++m_Next};
        inline static uint16_t m_Next{0};
    };

        void debug(){
            #ifdef DBG
            std::cout << " step : "<< std::to_string(++Next) <<"\n";
            #endif
        }     
        void debug(const std::string_view txt){                            
                std::cout << " step : "<< std::to_string(++Next)<< " :: "<<txt <<"\n";            
        }  
}
