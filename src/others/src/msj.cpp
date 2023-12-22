#include <others/src/msj.h>
#include<app/src/config.h>

namespace DEVICES{    
    void Msj_t::passMessage(QR::QR_OLED_BUFF* qr_oled_msjp){
        return ;
    }
        
    void Msj_t::get(){
        return;
    }

    void Msj_t::set(std::string& msj){
        return;
    }    
    
    void Msj_t::printData (bool flag , std::string_view msj){
        return;
    }
        
    void Msj_t::printQr   (bool flag , std::string_view msj){
        
        return;
    }
        
    void Msj_t::printOled (bool flag , std::string_view msj){
        #ifdef USE_OLED2
        auto qr_oled{std::make_unique<QR::Qr_img_t>()};
        qr_oled->create(msj);
        #endif
        return;
    }
        
}