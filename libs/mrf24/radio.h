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

#include <others/gpio.h>
#include <files/database.h>
#include <app/config.h>
#include <app/work.h>
//#include <oled/oled.h>
#include <data/data_analisis.h>



#ifdef USE_MAC_ADDRESS_LONG 
    #define MACADDR64
#elif defined (USE_MAC_ADDRESS_SHORT)
    #define MACADDR16
#endif




namespace OLED{
    struct Oled_t;
}

namespace MRF24J40{

   struct Radio_t
   {
        public:
            explicit Radio_t();
            ~Radio_t();

            void Run(bool&);
            void interrupt_routine();
        private :
            unsigned long last_time{0};
            unsigned long tx_interval{1000}; 
            bool status{false};
            bool flag {false};

        #ifdef ENABLE_INTERRUPT_MRF24 // rx
            //std::unique_ptr<GPIO::Gpio>pin_interrupt{}; 
            std::unique_ptr<DATABASE::Database_t>database{};
            //std::unique_ptr<FILESYSTEM::File_t>fs{};
            std::unique_ptr<WORK::Work_t>fs{};
           // std::unique_ptr<OLED::Oled_t>oled{};
        #else    
            //std::unique_ptr<QR::Qr_t>qr{};
            std::unique_ptr<WORK::Work_t>qr{};
        
        #endif
            std::unique_ptr<GPIO::Gpio>gpio{}; 
             DATA::BUFFER  buffer_receiver{};
    };

            void handle_tx();
            void handle_rx();



}//end MRF24J40
