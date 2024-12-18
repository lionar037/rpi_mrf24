// msj.h
#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <vector> // Asegúrate de incluir <vector>
#include <qr/include/qr.h>

#define MAX_LINES_SAVED 12

namespace DEVICES {

    struct Msj_t {
        Msj_t();
        ~Msj_t() = default;
        void get();
        void set(std::string_view);
        void Start() { system("clear"); }
        void passMessage(QR::QR_OLED_BUFF*);
        void printData();
        void printQr(bool, std::string_view);
        void printOled(bool, std::string_view);
        void setMaxLines(int);

    private:
        std::string m_txt;
        QR::QR_OLED_BUFF m_qr_oled_buff_msj;
        bool m_flag{ false };
        static std::vector<std::string> m_msj_memory; // Asegúrate de que el nombre sea consistente
    };

} // end DEVICES