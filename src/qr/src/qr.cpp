#include <iostream>
#include <cstring>
//#include <tuple>
#include <qr/include/qr.h>
#include <others/include/color.h>
#include <others/include/rfflush.h>
#include <vector>

namespace QR{


    bool Qr_t::create(const std::string_view& msj ) {
    auto            monitor     {std::make_unique <FFLUSH::Fflush_t>()};
        RST_COLOR() ;

        // Configuración del código QR
        QRcode* qr = QRcode_encodeString(msj.data(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);

        //auto qr = std::unique_ptr<QRcode>(QRcode_encodeString(fname.data(), 0, QR_ECLEVEL_L, QR_MODE_8, 1));
        // Imprime el código QR en la consola
        //std::cout << "\033[2J\033[H" << std::flush;
        //monitor->print("\n",1,10);
        SET_COLOR(SET_COLOR_WHITE_TEXT);
        std::cout << "\n";
        //std::vector<bool> buff;//(QRcode_getBufferSize(qr));
        //int index { 0 };
        for (int y = 0; y < qr->width; y++) {
            for (int x = 0; x < qr->width; x++) {                
               std::cout << (qr->data[y * qr->width + x] & 1 ? "██" : "  ");
                //buff.push_back((qr->data[index] & 1) != 0);
              
                //index++;
            }            
            std::cout << "\n";
        }
       
        // Libera la memoria
        // QRcode_free(qr.get()); 
        QRcode_free(qr); 
        return true;
    }




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
