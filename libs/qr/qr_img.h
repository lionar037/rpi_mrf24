#pragma once
#include <qrencode.h>

namespace QR_IMG{


    struct Qr_img_t{
        Qr_img_t();
        ~Qr_img_t();
        void saveQRCodeImage(const QRcode* , const char* );
        bool create();

    };
}