// msj.cpp

#include <others/include/msj.h>
#include <others/include/color.h>
#include <app/include/config.h>
//#include <vector>
//#include <string>

namespace DEVICES {

    int callGlobal{ 0 }; // Corrección
    int maxLinesGlobal{ 0 };
    
    // Aquí está la corrección: solo se define la variable sin 'static'
    std::vector<std::string> Msj_t::m_msj_memory; 

    void Msj_t::passMessage(QR::QR_OLED_BUFF* qr_oled_msjp) {
        return;
    }

    void Msj_t::get() {
        return;
    }

    void Msj_t::set(std::string_view msj_tmp) {
        m_msj_memory.push_back(std::string(msj_tmp)); // Usar el nombre correcto
        callGlobal++;
        if (callGlobal >= maxLinesGlobal) {
            m_msj_memory.clear();
            callGlobal = 0;
        }
    }

    void Msj_t::setMaxLines(int max) {
        maxLinesGlobal = max;
    }

    void Msj_t::printData() {
        SET_COLOR(SET_COLOR_CYAN_TEXT);
        for (const auto& text : m_msj_memory) { // Usar el nombre correcto
            std::cout << text;
        }
        m_msj_memory.clear(); // Usar el nombre correcto
    }

    void Msj_t::printQr(bool flag, std::string_view msj) {
        return;
    }

    void Msj_t::printOled(bool flag, std::string_view msj) {
        #ifdef USE_OLED2
        auto qr_oled = std::make_unique<QR::Qr_img_t>();
        qr_oled->create(msj);
        #endif
    }

    Msj_t::Msj_t() {
        m_msj_memory.reserve(100);
    }

}