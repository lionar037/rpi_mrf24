#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <qr/src/qr.h>

#define MAX_LINES_SAVED 12
namespace DEVICES{


    struct Msj_t{
         Msj_t();
        ~Msj_t()=default;
        void        get                 ( );
        void        set                 ( std::string& );
        void        Start               ( ){std::cout<<"Msj : Start() \n";}
        void        passMessage         ( QR::QR_OLED_BUFF* );
        void        printData           ( );
        void        printQr             ( bool , std::string_view );
        void        printOled           ( bool , std::string_view );
        void        setMaxLines         (int);
    private:
        std::string             m_txt;
        QR::QR_OLED_BUFF        m_qr_oled_buff_msj;
        bool                    m_flag { false };
        static std::vector<std::string> msj_memory{};
        static int m_call{ 0 };
        
    };



}