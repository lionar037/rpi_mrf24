#pragma once
#include <iostream>

#define MODULE_0 
//#define MODULE_1 
#ifdef MODULE_0
    #define ADDRESS 0x6001
    #define PAN_ID 0xCAFE
    #define ADDR_SLAVE 0x6002
    
#else
    #define ADDRESS 0x6002
    #define PAN_ID 0xCAFE
    #define ADDR_SLAVE  0x6001

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
