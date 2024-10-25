#pragma once
#include <iostream>


#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <gpio/include/gpio.h>
#include <files/include/database.h>
#include <app/include/config.h>
#include <app/include/work.h>
#include <app/include/data_analisis.h>
#include <qr/include/qr.h>
#include <security/include/security.h>

#include <memory>
#include <vector>
#include <sstream>
#include <iomanip>
#include <functional>
#include <cstdint>
#include <string>


#ifdef USE_MAC_ADDRESS_LONG 
    #define MACADDR64
#elif defined (USE_MAC_ADDRESS_SHORT)
    #define MACADDR16
#endif

#ifdef USE_OLED
    namespace OLED{
        struct Oled_t;
    }
#endif

#define POSITIOM_INIT_PRINTS 4

    namespace MOSQUITTO{
        struct Mosquitto_t;
    }


namespace MRF24J40{

   struct Radio_t
   {
        public:
            explicit            Radio_t();
                                ~Radio_t();
            void                Start(bool&);
            void                interrupt_routine();
            bool                Run(void);
            friend void         update();  

            static void         handle_tx();
            static void         handle_rx();

            void                funcion(std::function<void(uint8_t*)> rx);

        private :
            unsigned long       m_last_time{0};
            unsigned long       m_tx_interval{1000}; 
            bool                m_status{false};
            bool                m_flag {false};            
                                            
            
        #ifdef ENABLE_INTERRUPT_MRF24 // rx
            std::unique_ptr<DATABASE::Database_t>   database{};
            //std::unique_ptr<WORK::Work_t>           fs{};
            struct DATA::packet_rx                  buffer_receiver{};
        #else   //IS_TX
            #ifdef ENABLE_QR 
                std::unique_ptr<WORK::Work_t> qr{};                    
            #endif
        #endif             
        struct DATA::packet_tx                  buffer_transmiter{};
        std::unique_ptr <GPIO::Gpio_t> gpio{};    
        static std::unique_ptr < SECURITY::Security_t >     security;
        static std::unique_ptr < MOSQUITTO::Mosquitto_t >   mosq;  
        uint64_t add {0};   
        static std::string msj_txt;               
    };


}//end MRF24J40
