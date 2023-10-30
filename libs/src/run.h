#pragma once
#include <iostream>
#include <memory>


namespace MRF24J40{

   struct Run {
      public:
            explicit Run();
            ~Run();

            void loop();
            void interrupt_routine();
            //friend void handle_tx();
            //friend void handle_rx();
        private :
            unsigned long last_time{0};
            unsigned long tx_interval{0};        
    };

            void handle_tx();
            void handle_rx();



}//end MRF24J40
