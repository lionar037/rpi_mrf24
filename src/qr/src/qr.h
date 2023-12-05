#pragma once
#include <memory>
#include <qrencode.h>

#include <app/src/work.h>
#include <vector>
#include <app/src/data_analisis.h>
#include <tuple>
#include <string_view>

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
            bool                create                  (const std::string_view&);
            //template <typename T>
            //const T* create_qr (const char* /*,  std::vector<unsigned char>&*/) ;
            
            //unsigned char*      get_buffer_pointer      (std::vector<unsigned char>&); 
        private:
            std::vector<unsigned char>vs;  
            std::unique_ptr<QR_OLED> QrOled;
    };


    struct QrOled_t 
    {
            QrOled_t()=default;
            ~QrOled_t()=default;
            
            template <typename T>
            //const T* 
            void create_qr (std::string_view& str_view , std::vector<unsigned char>& tp) {
                T variavle{};
                //return std::tuple{true};
            }
    
    

            // template <typename T>
            // auto* create_qr_ (std::string_view& str_view , std::vector<unsigned char>&) {
                // return std::tuple{0};
            // }
            
            std::unique_ptr<QR_OLED> QrOled;
    };



    struct Qr_img_t : public WORK::Work_t
    {
            Qr_img_t();
            ~Qr_img_t();
            void    saveQRCodeImage     (const QRcode* , const char* );
            bool    create              (const std::string_view&);
         
        private :
            std::unique_ptr<TYME::Time_t>tyme{};
    };
}