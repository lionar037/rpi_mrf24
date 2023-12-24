#include <iostream>
#include <thread>
#include <vector>
#include <run/src/run.h>
#include <run/src/radio.h>
#include <others/src/msj.h>
#include <app/src/config.h>
#include <mrf24/src/mrf24j40.h>
#include <qr/src/qr.h>

//#if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4))
 #ifdef USE_MRF24_RX
    #include <display/src/oled.h>
#endif

 
namespace MRF24J40{
    extern std::string msj_txt;
}


namespace RUN{
    extern  QR::QR_OLED_BUFF codeQrGlobal;
    extern MRF24J40::Mrf24j mrf24j40_spi ;    
    
void Run_t::start()
{
    bool flag{true};
    
        system("clear"); 
    
     try    {
                auto mrf { std::make_unique<MRF24J40::Radio_t>()};        // Inicializar hilos y ejecutar las clases en paralelo
                auto msj { std::make_unique<DEVICES::Msj_t>()};  
            
                #ifdef USE_MRF24_RX                 
                static auto display { std::make_unique<OLED::Oled_t>() };    //inicializar una sola vez 
                #endif          
                                 
                //std::thread thread1([mrf = std::move(mrf)]() {});
                
                std::thread thread2(&DEVICES::Msj_t::Start, msj.get());

                #ifdef USE_MRF24_RX            
                std::thread thread3(&OLED::Oled_t::init , display.get());
                #endif
                                                
                //Esperar a que todos los hilos terminen
                //thread1.join();
                thread2.join();

                #ifdef USE_MRF24_RX
                thread3.join();                                
                #endif

    #ifdef USE_MRF24_RX     
    display->create(MRF24J40::msj_txt.c_str());
    while(true)
    #endif
    {                                

        flag= mrf->Run();     
        #ifdef USE_MRF24_RX
        if(flag==true){
                //display->create(MRF24J40::msj_txt.c_str());

                const auto x = QR::codeQrGlobal.height;
                const auto y = QR::codeQrGlobal.width;
                const auto z = QR::codeQrGlobal.data;
                const bool d = true;
                display->Graphics(x,y,d);
            }
        #endif                                
    }

                
            }
        catch(...){
                    std::cerr<<"\nerror :(\n";
        }
    }
}