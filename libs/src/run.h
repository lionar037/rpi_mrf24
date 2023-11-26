#pragma once
#include <iostream>
#include <memory>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <sstream>
#include <iomanip>

#include <gpio.h>
#include <database.h>

#define USE_MRF24_TX
//#define USE_MRF24_RX
#define USE_MAC_ADDRESS_LONG
//#define USE_MAC_ADDRESS_SHORT


#ifdef USE_MRF24_TX
    #define MODULE_TX 
#else
    #define MODULE_RX 
#endif

#ifdef USE_MAC_ADDRESS_LONG 
    #define MACADDR64
#elif defined (USE_MAC_ADDRESS_SHORT)
    #define MACADDR16
#endif

//#define MSJ "@ABCDEFGHIJKLMKNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz0123456@ABCDEFGHIJKLMKNOPQRS" 
#define MSJ "@ABCDEFGHIJKLMNO"
#ifdef MODULE_TX
    #define ADDRESS_LONG 0x1122334455667702
    #define ADDRESS_LONG_SLAVE 0x1122334455667701
    #define ADDRESS 0x6001
    #define PAN_ID 0x1234
    #define ADDR_SLAVE 0x6002
    #define MRF24_TRANSMITER_ENABLE
//#endif    
#elif  defined(MODULE_RX)
    #define ADDRESS_LONG 0x1122334455667701
    #define ADDRESS_LONG_SLAVE 0x1122334455667702
    #define ADDRESS 0x6002
    #define PAN_ID 0x1235
    #define ADDR_SLAVE  0x6001
    #define MRF24_RECEIVER_ENABLE
    #define ENABLE_INTERRUPT_MRF24
    
#endif


// namespace DATABASE{
    // struct Database_t;
// }

namespace MRF24J40{
    //struct GPIO::Gpio;

   struct Run //:public GPIO::Gpio
   {
        public:
            explicit Run();
            ~Run();

            void loop();
            void interrupt_routine();
        private :
            unsigned long last_time{0};
            unsigned long tx_interval{1000}; 
            bool status{false};

        #ifdef ENABLE_INTERRUPT_MRF24 // rx
            std::unique_ptr<GPIO::Gpio>pin_interrupt{}; 
            std::unique_ptr<DATABASE::Database_t>database{};
        #else
            std::unique_ptr<GPIO::Gpio>pin_only_output{}; 
        #endif 
      
    };

            void handle_tx();
            void handle_rx();



}//end MRF24J40
