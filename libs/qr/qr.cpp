#include <iostream>
#include <qr/qr.h>

namespace QR{

    Qr_t::Qr_t(){
        std::cout<<"Qr_t()\n";
    }

    Qr_t::~Qr_t(){
        std::cout<<"~Qr_t()\n";
    }
}