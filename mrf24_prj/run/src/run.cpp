
#include <qr/include/qr.h>
#include <run/include/run.h>
#include <run/include/radio.h>
#include <others/include/msj.h>
#include <app/include/config.h>
#include <mrf24/include/mrf24j40.h>
#include <network/include/hostname.h>
 #ifdef USE_MRF24_RX
    #include <display/include/oled.h>
#endif
#include <iostream>
#include <thread>
#include <vector>

namespace DEBUGGER{
    extern void debug();
    extern void debug(const std::string_view);
}
 
namespace MRF24J40{
    extern std::string msj_txt;
}

namespace QR{
    extern QR_OLED_BUFF codeQrGlobal;
}

namespace RUN
{
    //extern MRF24J40::Mrf24j mrf24j40_spi ;    
    
int Run_t::start(){
[[gnu::unused]]        bool flag{true};
       // system("clear"); 
    
    try{
            auto zigbee { std::make_unique<MRF24J40::Radio_t>()};        // Inicializar hilos y ejecutar las clases en paralelo            
            auto msj { std::make_unique<DEVICES::Msj_t>()};  
            auto ip { std::make_unique<NETWORK::Hostname_t>()};  
        
            #ifdef USE_MRF24_RX                 
            ///static auto oled { std::make_unique<OLED::Oled_t>() };    //inicializar una sola vez 
             auto  oled = std::make_unique<OLED::Oled_t>(128, 64, BCM2835_I2C_CLOCK_DIVIDER_626, 0x3C);
        
                if (!oled->begin()) {
                    return -1;
                }        
                oled->clearScreen();
                oled->displayText("MRF24J40 ZigBee ", 0, 0);
                TYME::delay_ms(3000);
                //bcm2835_delay(900);
                #endif  

            std::thread thread2(&DEVICES::Msj_t::Start, msj.get());
            #ifdef USE_MRF24_RX            
            std::thread thread3(&OLED::Oled_t::init , oled.get());
            #endif            
            //Esperar a que todos los hilos terminen
                                         
            //std::thread thread1([mrf = std::move(mrf)]() {});            
            //thread1.join();
            thread2.join();            
            DEBUGGER::debug("thread2.join"); 

            ip->GetHostname( MRF24J40::msj_txt);
            const auto host =MRF24J40::msj_txt;
            DEBUGGER::debug(host); 
            #ifdef USE_MRF24_RX
                thread3.join();                                                        
                oled->create(MRF24J40::msj_txt.c_str());
                while(true)
            #endif
            {   
                DEBUGGER::debug("flag = zigbee->Run");                              
                flag = zigbee->Run();     
                #ifdef USE_MRF24_RX
                if(flag==true){                
                    const auto x = QR::codeQrGlobal.height;
                    const auto y = QR::codeQrGlobal.width;                                            
                    oled->Graphics(x,y,QR::codeQrGlobal.data,QR::codeQrGlobal.bufferComplete);
                }
                #endif                                
            }                
        }//end try
        catch(...){
                    std::cerr<<"\nerror :(\n";
        }
    return 0;
    }
}
