
//#include <qr/src/qr.h>
#include <run/include/run.h>
#include <run/include/radio.h>
#include <others/include/msj.h>
#include <app/include/config.h>
#include <mrf24/include/mrf24j40.h>
#include <network/include/hostname.h>
#include <iostream>
#include <thread>
#include <vector>

//#if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4))
 #ifdef USE_MRF24_RX
    #include <display/include/oled.h>
#endif

 
namespace MRF24J40{
    extern std::string msj_txt;
}

namespace QR{
    //#ifdef USE_OLED
    extern QR_OLED_BUFF codeQrGlobal;
    //#endif
}

namespace RUN{
    extern MRF24J40::Mrf24j mrf24j40_spi ;    
    
void Run_t::start()
{
        [[gnu::unused]]  bool flag{true};
        system("clear"); 
    
    try{
            auto mrf { std::make_unique<MRF24J40::Radio_t>()};        // Inicializar hilos y ejecutar las clases en paralelo
            auto msj { std::make_unique<DEVICES::Msj_t>()};  
            auto ip { std::make_unique<NETWORK::Hostname_t>()};  
        
            #ifdef USE_OLED                 
                static auto oled { std::make_unique<OLED::Oled_t>() };    //inicializar una sola vez 
            #endif          

            std::thread thread2(&DEVICES::Msj_t::Start, msj.get());
            #ifdef USE_OLED            
            std::thread thread3(&OLED::Oled_t::init , oled.get());
            #endif            
            //Esperar a que todos los hilos terminen
                                         
            //std::thread thread1([mrf = std::move(mrf)]() {});            
            //thread1.join();
            thread2.join();            
            
            ip->GetHostname(MRF24J40::msj_txt);
            #ifdef USE_MRF24_RX
            #ifdef USE_OLED  
                thread3.join(); 
            #endif
            #ifdef USE_OLED                                                       
                oled->create(MRF24J40::msj_txt.c_str());
            #endif
            while(true)
            #endif
            {                                
                flag= mrf->Run();     
                #ifdef USE_MRF24_RX
                if(flag==true){                                                        
                    #ifdef USE_OLED
                        const auto x = QR::codeQrGlobal.height;
                        const auto y = QR::codeQrGlobal.width;                                            
                        oled->Graphics(x,y,QR::codeQrGlobal.data,QR::codeQrGlobal.bufferComplete);
                    #endif
                }
                #endif                                
            }

                
        }//end try
        catch(...){
                    std::cerr<<"\nerror :(\n";
        }
    }
}