#pragma once

#include <app/include/config.h>
#include <iomanip>
#include <type_traits>
#include <iostream>
#include <cstdint>

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
    
    template<typename T>
    void printHex(T value) {
        // Asegurarse de que el tipo T sea un entero sin signo
        static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value, "El tipo debe ser un entero sin signo");

        // Calcular el número de dígitos necesarios en función del tamaño del tipo
        int width = sizeof(T) * 2; // Cada byte son 2 dígitos hexadecimales

        // Imprimir el valor en hexadecimal
//      std::cout << "0x" << std::hex << std::setw(width) << std::setfill('0') << static_cast<uint64_t>(value) << std::endl;
        std::cout << "0x" << std::hex << std::setw(width) << std::setfill('0') << static_cast<uint64_t>(value) ;
    }

}

