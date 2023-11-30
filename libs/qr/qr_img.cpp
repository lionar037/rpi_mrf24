#include <iostream>
#include <cstring>
#include <fstream>
#include <png.h>
#include <zlib.h>
#include <qr/qr.h>
#include <others/color.h>

namespace QR{


void Qr_img_t::saveQRCodeImage(const QRcode* qr, const char* filename) {
    // Abre el archivo para escritura en formato binario
    std::ofstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo para escritura." << std::endl;
        return;
    }

    // Configuración de la imagen PNG
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info = png_create_info_struct(png);

    // Configura la función de escritura
    png_set_write_fn(png, static_cast<void*>(&file), [](png_structp png, png_bytep data, png_size_t length) {
        static_cast<std::ostream*>(png_get_io_ptr(png))->write(reinterpret_cast<char*>(data), length);
    }, nullptr);

    // Configura la información de la imagen PNG
    png_set_IHDR(png, info, qr->width, qr->width, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Configura el método de filtro
    png_set_filter(png, 0, PNG_FILTER_NONE);

    // Escribir datos del código QR en la imagen PNG
    png_write_info(png, info);
    png_bytep row = new png_byte[png_get_rowbytes(png, info)];
    for (int y = 0; y < qr->width; ++y) {
        for (int x = 0; x < qr->width; ++x) {
            png_bytep pixel = &row[x * 4];
            pixel[0] = pixel[1] = pixel[2] = (qr->data[y * qr->width + x] & 1) ? 0 : 255;
            pixel[3] = 255;
        }
        png_write_row(png, row);
    }

    // Finaliza la escritura de la imagen
    png_write_end(png, info);
    png_destroy_write_struct(&png, &info);

    // Libera la memoria
    delete[] row;
}

bool Qr_img_t::create(const char* data ) {
    // Datos que deseas codificar en el QR
    //const char* data = " Mi QR ";
        SET_COLOR(SET_COLOR_WHITE_TEXT);
    // Configuración del código QR
    QRcode* qr = QRcode_encodeString(data, 0, QR_ECLEVEL_L, QR_MODE_8, 1);

    // Imprime el código QR en la consola
    for (int y = 0; y < qr->width; y++) {
        for (int x = 0; x < qr->width; x++) {
            std::cout << (qr->data[y * qr->width + x] & 1 ? "██" : "  ");
        }
        std::cout << std::endl;
    }

    // Guarda el código QR como imagen PNG
    saveQRCodeImage(qr, "mi_qr.png");

    // Libera la memoria
    QRcode_free(qr);

    return true;
}


Qr_img_t::Qr_img_t(){std::cout<<"Qr_img_t()\n";}
Qr_img_t::~Qr_img_t(){std::cout<<"~Qr_img_t()\n";}

}
