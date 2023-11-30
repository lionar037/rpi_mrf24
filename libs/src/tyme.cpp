#include <tyme.h>
#include <chrono>
#include <thread>
#include <config.h>
//#include <unistd.h> // Para usleep

namespace TYME{

    Time_t::Time_t(){
        #ifdef DBG
            std::cout << " Time_t()\n";
        #endif

    }

    Time_t::~Time_t(){
        #ifdef DBG
            std::cout << " ~Time_t()\n";
        #endif
    }

    void Time_t::delay_ms(const uint32_t t){
        std::chrono::microseconds delay_loc(t);
        std::this_thread::sleep_for(delay_loc);
        //     usleep(t);//otra forma
    return;
    }
}