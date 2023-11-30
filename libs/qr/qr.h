#pragma once
#include <qrencode.h>

namespace QR{

    struct Qr_t{
            Qr_t()=default;
            ~Qr_t()=default;
            bool create(const char* );
        private:
    };

    struct Qr_img_t
    {
        Qr_img_t();
        ~Qr_img_t();

        void saveQRCodeImage(const QRcode* , const char* );
        bool create(const char*);
    };

}