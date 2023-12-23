#include <iostream>
#include <thread>
#include <vector>
//#define USE_OLED2

#include <run/src/run.h>
#include <run/src/radio.h>
#include <others/src/msj.h>
#include <app/src/config.h>
//#if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4))
 #ifdef USE_OLED2
    #include <display/src/oled.h>
#endif

 
namespace MRF24J40{
    extern Mrf24j mrf24j40_spi ;
    extern bool m_flag_rx_enable_msj{false};
}

namespace RUN{


void Run_t::start()
{
    std::string msj_txt="@ABCDEFGHIJKLMNO1234567890#";
        system("clear"); 
    
     try    {
                auto mrf { std::make_unique<MRF24J40::Radio_t>()};        // Inicializar hilos y ejecutar las clases en paralelo
                auto msj { std::make_unique<DEVICES::Msj_t>()};  
            
                #ifdef USE_OLED2                 
                static auto display { std::make_unique<OLED::Oled_t>() };    //inicializar una sola vez 
                #endif          

                std::thread thread1(&MRF24J40::Radio_t::Run, mrf.get());
                std::thread thread2(&DEVICES::Msj_t::Start, msj.get());

                #ifdef USE_OLED2            
                std::thread thread3(&OLED::Oled_t::init , display.get());
                #endif

                //Esperar a que todos los hilos terminen
                thread1.join();
                thread2.join();

                #ifdef USE_OLED2
                thread3.join();
                std::cout<<"MSJ OLED : \n";
                
                #endif
while(true){
                        //std::cout << "\033[2J\033[H" << std::flush;
        //system("clear");
        display->create(msj_txt.c_str());
        //gpio->app(m_flag);              
        //mrf24j40_spi.interrupt_handler();
        //Start(m_flag);  
     }

                
            }
        catch(...){
                    std::cerr<<"\nerror :(\n";
        }
    }
}