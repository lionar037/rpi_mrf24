#pragma once
#include <iostream>
#include <memory>


namespace MRF24J40{

//struct Mrf24j;
 

   struct Run {
            explicit Run();
            ~Run();

            void loop();
            void interrupt_routine();
            friend void handle_tx();
            friend void handle_rx();
        private :
           // std::unique_ptr<Mrf24j> mrf24j40_spi {};
            unsigned long last_time{0};
            unsigned long tx_interval{0};        
    };

            void handle_tx();
            void handle_rx();



}//end MRF24J40
