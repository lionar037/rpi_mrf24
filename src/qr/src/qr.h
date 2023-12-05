#pragma once
#include <memory>
#include <qrencode.h>
#include <app/src/work.h>
#include <vector>

namespace TYME{
    struct Time_t;
}

namespace QR{
    struct Qr_t : public WORK::Work_t
    {
            Qr_t()=default;
            ~Qr_t()=default;
            bool        create                  (const char* );
            bool        create_qr              (const char*,  std::vector<unsigned char*>&) ;
        private:
    };

    struct Qr_img_t : public WORK::Work_t
    {
            Qr_img_t();
            ~Qr_img_t();
            void    saveQRCodeImage     (const QRcode* , const char* );
            bool    create              (const char*);
         
        private :
            std::unique_ptr<TYME::Time_t>tyme{};
    };
}