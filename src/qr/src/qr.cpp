#include <iostream>
#include <cstring>
#include <qr/src/qr.h>
#include <others/src/color.h>

namespace QR{

    bool Qr_t::create(const char* data ) {
    
        SET_COLOR(SET_COLOR_WHITE_TEXT);
        // Configuración del código QR
        QRcode* qr = QRcode_encodeString(data, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
        
std::cout<<"qr->width : " <<qr->width<<"\n";


        // Imprime el código QR en la consola
        for (int y = 0; y < qr->width; y++) {
            for (int x = 0; x < qr->width; x++) {
                std::cout << (qr->data[y * qr->width + x] & 1 ? "██" : "  ");
            }
            std::cout << std::endl;
        }
    
        // Libera la memoria
        QRcode_free(qr);
        
        return true;
    }


const unsigned char* Qr_t::create_qr(const char* data, std::vector<unsigned char>& vt) {
    std::cout<<"\n";
    QRcode* qr = QRcode_encodeString(data, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    std::cout <<"\nqr->width : "<<qr->width<<"\n";
    for (int y = 0; y < qr->width; y++) {
        for (int x = 0; x < qr->width; x++) {
            vt.push_back((qr->data[y * qr->width + x] & 1) ? 1 : 0); 
        }
    }
    uint8_t count=0;
    std::cout<<"\n";
    for(const auto& byt : vt) 
    {
    if(count>64){count=0;std::cout<<"\n";}
        printf("%x,",byt);
        count++;
    }
    QRcode_free(qr);
    return vt.data();
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