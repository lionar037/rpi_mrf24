#pragma once
#include <memory>


namespace MRF24J40{
   // using Mrf24j_t = std::unique_ptr<Mrf24j> ;
struct Mrf24j;

    struct Run //: public Mrf24j 
    {
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

             std::unique_ptr<Mrf24j> mrf24j40_spi ;
    };

            void handle_tx();
            void handle_rx();



}//end MRF24J40
