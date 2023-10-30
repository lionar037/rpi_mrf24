#include <tyme.h>

#include <chrono>
#include <thread>
#include <unistd.h> // Para usleep

namespace TYME{

    Time_t::Time_t(){
        std::cout << " Time_t()\n";

    }

    Time_t::~Time_t(){

        std::cout << " ~Time_t()\n";
    }

    void Time_t::delay_ms(const uint32_t t){
    //    std::chrono::microseconds delay_loc(t);
      //  std::this_thread::sleep_for(delay_loc);
        //   usleep(t);//otra forma
        return;
    }
}