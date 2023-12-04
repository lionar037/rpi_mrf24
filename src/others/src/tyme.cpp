#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

#include <app/src/config.h>
#include <others/src/tyme.h>

//#include <unistd.h> // Libreria para usleep

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

    const std::string  Time_t::get_tyme()
    {

        auto now = std::chrono::system_clock::now();

        // Convertir el tiempo actual a una estructura de tiempo local
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&currentTime);

        // Formatear la fecha y hora según tu especificación
        std::ostringstream oss;
            std::cout<<"\n";
        oss << std::put_time(&localTime, "%Y%m%d%H%M%S");
            std::cout<<"\n";
        std::string tyme = oss.str();
        return tyme;
    }
}