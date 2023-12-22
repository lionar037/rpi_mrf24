#include <iostream>
#include <thread>
#include <vector>
#define USE_OLED2

#include <run/src/run.h>
#include <run/src/radio.h>
#include <others/src/msj.h>
#include <app/src/config.h>
//#if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4))
 #ifdef USE_OLED2
    #include <display/src/oled.h>
#endif

 

namespace RUN{

void Run_t::start()
{
    std::string msj;
        system("clear"); 
    
     try    {
                auto mrf { std::make_unique<MRF24J40::Radio_t>()};        // Inicializar hilos y ejecutar las clases en paralelo
                auto msj { std::make_unique<DEVICES::Msj_t>()};  
            
                #ifdef USE_OLED2                 
                static auto diplay { std::make_unique<OLED::Oled_t>() };    //inicializar una sola vez 
                #endif          

                std::thread thread1(&MRF24J40::Radio_t::Run, mrf.get());
                std::thread thread2(&DEVICES::Msj_t::Start, msj.get());

                #ifdef USE_OLED2            
                std::thread thread3(&OLED::Oled_t::init , diplay.get());
                #endif

                //Esperar a que todos los hilos terminen
                thread1.join();
                thread2.join();

                #ifdef USE_OLED2
                thread3.join();
                #endif

            }
        catch(...){
                    std::cerr<<"\nerror :(\n";
        }
    }
}