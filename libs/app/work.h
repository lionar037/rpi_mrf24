#pragma once
#include <files/file.h>
#include <qr/qr.h>

namespace FILESYSTEM{
        struct File_t;
    }

    namespace QR{
        struct Qr_img_t;
    }

namespace WORK{

    struct Work_t: public FILESYSTEM::File_t , QR::Qr_img_t{
            virtual bool    create(const char*)=0;       
            virtual         ~Work_t()=default;
        private:
    };
}