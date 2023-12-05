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



bool Qr_t::create_qr(const char* data, unsigned char* buffer/*[64][64]*/) 
    {

        SET_COLOR(SET_COLOR_WHITE_TEXT);
        // Configuración del código QR
        QRcode* qr = QRcode_encodeString(data, 0, QR_ECLEVEL_L, QR_MODE_8, 1);

        std::cout << "qr->width : " << qr->width << "\n";

        // Rellenar el buffer de píxeles con datos de código QR
        for (int y = 0; y < qr->width; y++) {
            for (int x = 0; x < qr->width; x++) {
                buffer[y][x] = (qr->data[y * qr->width + x] & 1) ? 1 : 0;  // 1 para píxel negro, 0 para píxel blanco
            }
        }

        // Libera la memoria
        QRcode_free(qr);

        return true;
    }
    
}