#pragma once
#include <iostream>
#include <memory>
#include <gpio.h>

#define MODULE_TX 
//#define MODULE_RX 

#define MSJ "\
@ABCDEFGHIJKLMKNOPQR\
STUVWXYZ0123456789ab\
cdefghijklmnopqrstuvw\
xyz|#¢∞¬÷(@ABCDEFGHIJK\
LMKNOPQRSTUVWXYZ0123456789ABs" 

#ifdef MODULE_TX
    #define ADDRESS 0x6001
    #define PAN_ID 0x1234
    #define ADDR_SLAVE 0x6002
    #define MRF24_TRANSMITER_ENABLE
#endif    

#ifdef MODULE_RX
    #define ADDRESS 0x6002
    #define PAN_ID 0x1234
    #define ADDR_SLAVE  0x6001
    #define MRF24_RECEIVER_ENABLE
    #define ENABLE_INTERRUPT_MRF24
    
#endif


namespace MRF24J40{
    //struct GPIO::Gpio;

   struct Run :public GPIO::Gpio{
        public:
            explicit Run();
            ~Run();

            void loop();
            void interrupt_routine();
        private :
            unsigned long last_time{0};
            unsigned long tx_interval{1000}; 
        #ifdef ENABLE_INTERRUPT_MRF24
            std::unique_ptr<GPIO::Gpio>pin_interrupt{}; 
        #endif 
      
    };

            void handle_tx();
            void handle_rx();



}//end MRF24J40
