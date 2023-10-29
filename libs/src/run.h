#pragma once
#include <memory>
#include <mrf24j40.h>

namespace MRF24J40{
   // using Mrf24j_t = std::unique_ptr<Mrf24j> ;

    struct Run //: public Mrf24j 
    {
            explicit Run();
            ~Run();

            void loop();
            void interrupt_routine();
             void handle_tx();
             void handle_rx();
        private :
           // std::unique_ptr<Mrf24j> mrf24j40_spi {};
            unsigned long last_time{0};
            unsigned long tx_interval{0};

            
    };



}//end MRF24J40
