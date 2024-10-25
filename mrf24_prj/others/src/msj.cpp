#include <vector>
#include<string>
#include <others/include/msj.h>
#include <others/include/color.h>
#include<app/include/config.h>

namespace DEVICES{    

int callGobal{ 0 };
int maxLinesGlobal{0};
static std::vector<std::string> Msj_t::msj_memory;

    void Msj_t::passMessage(QR::QR_OLED_BUFF* qr_oled_msjp){
        return ;
    }
        
    void Msj_t::get(){
        return;
    }

    void Msj_t::set(std::string_view msj_tmp){
        
        msj_memory.push_back(msj_tmp.data());  
         
        callGobal++;     
        if(callGobal>=maxLinesGlobal){
            msj_memory.clear();
            callGobal=0;
        }
        
        return;
    }    
    void Msj_t::setMaxLines(int max)
    {
        maxLinesGlobal = max;
    }
    
    void Msj_t::printData ()
    {
        SET_COLOR(SET_COLOR_CYAN_TEXT);
        for (const auto& text : msj_memory) {
            std::cout << text ;
        }
        msj_memory.clear();
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

    Msj_t::Msj_t()    
    {
        msj_memory.reserve(100);
    }
        
}