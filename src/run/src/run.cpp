#include <iostream>
#include <thread>
#include <vector>
//#include <qr/src/qr.h>
#include <run/src/run.h>
#include <run/src/radio.h>
#include <others/src/msj.h>
#include <app/src/config.h>
#include <mrf24/src/mrf24j40.h>
#include <network/src/hostname.h>


//#if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4))
 #ifdef USE_MRF24_RX
    #include <display/src/oled.h>
#endif

 
namespace MRF24J40{
    extern std::string msj_txt;
}

namespace QR{
    extern QR_OLED_BUFF codeQrGlobal;
}

namespace RUN{
    extern MRF24J40::Mrf24j mrf24j40_spi ;    
    
void Run_t::start()
{
        bool flag{true};
        system("clear"); 
    
    try{
            auto mrf { std::make_unique<MRF24J40::Radio_t>()};        // Inicializar hilos y ejecutar las clases en paralelo
            auto msj { std::make_unique<DEVICES::Msj_t>()};  
            auto ip { std::make_unique<NETWORK::Hostname_t>()};  
        
            #ifdef USE_MRF24_RX                 
            static auto oled { std::make_unique<OLED::Oled_t>() };    //inicializar una sola vez 
            #endif          
                             
            //std::thread thread1([mrf = std::move(mrf)]() {});
            
            std::thread thread2(&DEVICES::Msj_t::Start, msj.get());
            
            #ifdef USE_MRF24_RX            
            std::thread thread3(&OLED::Oled_t::init , oled.get());
            #endif
            //std::thread thread4(&NETWORK::Hostname_t::print, ip.get());                                
            //Esperar a que todos los hilos terminen
            //thread1.join();
            thread2.join();
            //thread4.join();
            //ip->print();
            ip->GetHostname(MRF24J40::msj_txt);
            #ifdef USE_MRF24_RX
            thread3.join();                                                        
            oled->create(MRF24J40::msj_txt.c_str());
            while(true)
            #endif
            {                                
                flag= mrf->Run();     
                #ifdef USE_MRF24_RX
                if(flag==true){                
                    auto x = QR::codeQrGlobal.height;
                    auto y = QR::codeQrGlobal.width;                                            
                    oled->Graphics(x,y,QR::codeQrGlobal.data,QR::codeQrGlobal.bufferComplete);
                }
                #endif                                
            }

                
        }//end try
        catch(...){
                    std::cerr<<"\nerror :(\n";
        }
    }
}