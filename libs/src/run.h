#pragma once
#include <iostream>
//#include <memory>
#define MODULE_0 
//#define MODULE_1 
#ifdef MODULE_0
    #define ADDR_SETTINGS 0xCAFE
    #define ADDR_SLAVE 0xCAFD
#else
    #define ADDR_SETTINGS 0xCAFD
    #define ADDR_SLAVE 0xCAFE
#endif


namespace MRF24J40{

   struct Run {
        public:
            explicit Run();
            ~Run();

            void loop();
            void interrupt_routine();
        private :
            unsigned long last_time{0};
            unsigned long tx_interval{1000};        
    };

            void handle_tx();
            void handle_rx();



}//end MRF24J40
