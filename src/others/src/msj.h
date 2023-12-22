#pragma once
#include <iostream>
#include <string>
#include <qr/src/qr.h>


namespace DEVICES{


    struct Msj_t{
         Msj_t(){}
        ~Msj_t(){}
        void        get                 ( );
        void        set                 ( std::string& );
        void        Start               ( ){std::cout<<"Msj : Start() \n";}
        void        passMessage         ( QR::QR_OLED_BUFF* );
        void        printData           ( bool , std::string_view );
        void        printQr             ( bool , std::string_view );
        void        printOled           ( bool , std::string_view );

    private:
        std::string             txt;
        QR::QR_OLED_BUFF        m_qr_oled_buff_msj;
        bool                    flag { false };
    };



}