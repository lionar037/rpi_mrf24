#include <iostream>
#include <thread>
#include <vector>
#include <run/src/run.h>
#include <run/src/radio.h>
#include <others/src/msj.h>
#include <app/src/config.h>
#include <mrf24/src/mrf24j40.h>
//#if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4))
 #ifdef USE_OLED2
    #include <display/src/oled.h>
#endif

 
namespace MRF24J40{
    extern Mrf24j mrf24j40_spi ;
    extern bool m_flag_rx_enable_msj;
}

namespace RUN{


void Run_t::start()
{
    bool flag{true};
    std::string msj_txt="@ABCDEFGHIJKLMNO1234567890#";
        system("clear"); 
    
     try    {
                auto mrf { std::make_unique<MRF24J40::Radio_t>()};        // Inicializar hilos y ejecutar las clases en paralelo
                auto msj { std::make_unique<DEVICES::Msj_t>()};  
            
                #ifdef USE_OLED2                 
                static auto display { std::make_unique<OLED::Oled_t>() };    //inicializar una sola vez 
                #endif          
                                
                //std::thread thread1([mrf = std::move(mrf)]() {});
                
                std::thread thread2(&DEVICES::Msj_t::Start, msj.get());

                #ifdef USE_OLED2            
                std::thread thread3(&OLED::Oled_t::init , display.get());
                #endif
                
                //std::thread hiloA([class_a = std::move(class_a)]() {});
                
                //Esperar a que todos los hilos terminen
                //thread1.join();
                thread2.join();

                #ifdef USE_OLED2
                thread3.join();                                
                #endif
    #ifdef USE_MRF24_RX     
    while(true)
    #endif
    {                                
        #ifdef USE_MRF24_RX
        if(flag==true)display->create(msj_txt.c_str());
        #endif
        flag= mrf->Run();                                     
    }

                
            }
        catch(...){
                    std::cerr<<"\nerror :(\n";
        }
    }
}