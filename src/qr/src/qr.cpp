#include <iostream>
#include <cstring>
//#include <tuple>
#include <qr/src/qr.h>
#include <others/src/color.h>
#include <others/src/rfflush.h>


namespace QR{

    bool Qr_t::create(const std::string_view& fname ) {
    auto            monitor     {std::make_unique <FFLUSH::Fflush_t>()};
        RST_COLOR() ;
        SET_COLOR(SET_COLOR_WHITE_TEXT);
        // Configuración del código QR
        QRcode* qr = QRcode_encodeString(fname.data(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);
        // Imprime el código QR en la consola
        std::cout << "\033[2J\033[H" << std::flush;
        monitor->set(" ",30,0);
        std::cout << "\n";
        for (int y = 0; y < qr->width; y++) {
            for (int x = 0; x < qr->width; x++) {
                std::cout << (qr->data[y * qr->width + x] & 1 ? "██" : "  ");
            }
            //std::cout << std::endl;
            std::cout << "\n";
        }
    
        // Libera la memoria
        QRcode_free(qr); 
        return true;
    }

// template <typename T>
// const T* QrOled_t::create_qr(const std::string_view& str_view, std::vector<unsigned char>& vt) 
// {
    // 
    // std::cout<<"\n";
//     QRcode* qr = QRcode_encodeString(str_view.data(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    
//     for (int y = 0; y < qr->width; y++) {
//         for (int x = 0; x < qr->width; x++) {
//          vt.push_back((qr->data[y * qr->width + x] & 1) ? 1 : 0); 
//         }
//     }

//     QRcode_free(qr);
  
// QrOled = std::make_unique<T>();
// QrOled->height=33;
// QrOled->width=33;
//    return {QrOled.release()};//vt.data();
// return std::tuple{true};//{reinterpret_cast<const T>(QrOled)};
// }



//  unsigned char*  Qr_t::create_qr(const char* data, std::vector<unsigned char>& vt) 
//     {

//         SET_COLOR(SET_COLOR_WHITE_TEXT);
//         // Configuración del código QR
//         QRcode* qr = QRcode_encodeString(data, 0, QR_ECLEVEL_L, QR_MODE_8, 1);

//         std::cout << "qr->width : " << qr->width << "\n";

//         // Rellenar el buffer de píxeles con datos de código QR
//         for (int y = 0; y < qr->width; y++) {
//             for (int x = 0; x < qr->width; x++) {
//                // vt[y][x] = (qr->data[y * qr->width + x] & 1) ? 1 : 0;  // 1 para píxel negro, 0 para píxel blanco
//             vt.push_back((qr->data[y * qr->width + x] & 1) ? 1 : 0); 
//             //vs.push_back((qr->data[y * qr->width + x] & 1) ? 1 : 0); 
//             }
//         }

//         // Libera la memoria
//         QRcode_free(qr);

//         return vt.data();
//     }
    
   
}