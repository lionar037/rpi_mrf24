#pragma once
#include <memory>
#include <qrencode.h>

#include <app/src/work.h>
#include <vector>
 #include <app/src/data_analisis.h>


namespace TYME{
    struct Time_t;
}

namespace QR{


    typedef struct qr_oled{
            int width;
            int height;
            char data[(64*64/8)];
    }QR_OLED;


    struct Qr_t : public WORK::Work_t
    {
            Qr_t()=default;
            ~Qr_t()=default;
            bool                create                  (const char* );
            //template <typename T>
            //const T* create_qr (const char* /*,  std::vector<unsigned char>&*/) ;
            
            //unsigned char*      get_buffer_pointer      (std::vector<unsigned char>&); 
        private:
            std::vector<unsigned char>vs;  
            std::unique_ptr<qr_oled> QrOled;
    };


    struct QrOled_t 
    {
            QrOled_t()=default;
            ~QrOled_t()=default;
            
            template <typename T>
            const T* create_qr (const char* /*,  std::vector<unsigned char>&*/) ;
            
            //unsigned char*      get_buffer_pointer      (std::vector<unsigned char>&); 
            
            
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