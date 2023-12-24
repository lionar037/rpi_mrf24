#pragma once
#include <memory>
#include <tuple>
#include <vector>
#include <string_view>
#include <qrencode.h>
#include <app/src/work.h>
#include <app/src/data_analisis.h>


namespace TYME{
    struct Time_t;
}

namespace QR{

    typedef struct qr_oled{
            int width;
            int height;
            bool* data = nullptr;
    }QR_OLED_BUFF;


    struct Qr_t : public WORK::Work_t
    {
            Qr_t()=default;
            ~Qr_t()=default;
            bool                create                  (const std::string_view&);
        private:
            std::vector<unsigned char>vs;  
            std::unique_ptr<QR_OLED_BUFF> QrOled;
    };


     struct QrOled_t 
    {
            QrOled_t()=default;
            ~QrOled_t()=default;

            template <typename T>
            void create_qr (std::string_view& str_view ,  std::vector<T>& variable) {
                return;
            }          
    };       


    struct Qr_img_t : public WORK::Work_t
    {
            Qr_img_t();
            ~Qr_img_t();
            void    saveQRCodeImage     ( const QRcode* , const char* );
            bool    create              ( const std::string_view& );
            void    drawRectangle       ( int , int );
            bool    create2             ( const std::string_view& );
            void    print               ( );
        private :
            std::unique_ptr<TYME::Time_t>tyme{};            
    };

static QR_OLED_BUFF codeQrGlobal;
}