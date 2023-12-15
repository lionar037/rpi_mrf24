#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>
#include <iomanip>
#include <functional>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <others/src/gpio.h>
#include <files/src/database.h>
#include <app/src/config.h>
#include <app/src/work.h>
#include <app/src/data_analisis.h>
#include <qr/src/qr.h>
#include <security/src/security.h>

#ifdef USE_MAC_ADDRESS_LONG 
    #define MACADDR64
#elif defined (USE_MAC_ADDRESS_SHORT)
    #define MACADDR16
#endif

#ifdef USE_MRF24_RX
    namespace OLED{
        struct Oled_t;
    }
#endif

#define POSITIOM_INIT_PRINTS 4

    namespace MOSQUITTO{
        struct Mosquitto_t;
    }

    // namespace SECURITY{
        // struct Security_t;
    // }

namespace MRF24J40{

   struct Radio_t
   {
        public:
            explicit            Radio_t();
                                ~Radio_t();
            void                Init(bool&);
            void                interrupt_routine();
            void                Run(void);
            friend void         update();  

            static void         handle_tx();
            static void         handle_rx();

            void                funcion(std::function<void(uint8_t*)> rx);

        private :
            unsigned long       last_time{0};
            unsigned long       tx_interval{1000}; 
            bool                status{false};
            bool                flag {false};
                                            
            
        #ifdef ENABLE_INTERRUPT_MRF24 // rx
            std::unique_ptr<DATABASE::Database_t>   database{};
            //std::unique_ptr<WORK::Work_t>           fs{};
            struct DATA::packet_rx                  buffer_receiver{};
        #else    
            std::unique_ptr<WORK::Work_t> qr{};
            struct DATA::packet_tx                  buffer_transmiter{};
        
        #endif             
        
        std::unique_ptr<GPIO::Gpio> gpio{};    
        static std::unique_ptr < SECURITY::Security_t >     security;
        static std::unique_ptr < MOSQUITTO::Mosquitto_t >   mosq;    
        
        
    };


}//end MRF24J40
