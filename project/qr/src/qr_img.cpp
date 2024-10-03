#include <iostream>
#include <cstring>
#include <fstream>

extern "C"{
    #include <png.h>
    #include <zlib.h>    
}

#include <others/include/rfflush.h>
#include <others/include/color.h>
#include <app/include/config.h>
#include <others/include/tyme.h>
#include <qr/include/qr.h>

namespace QR{

QR_OLED_BUFF codeQrGlobal;

    void Qr_img_t::saveQRCodeImage(const QRcode* qr, const char* filename) {
        // Tamaño del borde blanco (en píxeles)
        const int borderSize = 10;

        // Tamaño total de la imagen (incluyendo el borde)
        const int imageSize = qr->width + 2 * borderSize;

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
        png_set_IHDR(png, info, imageSize, imageSize, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        // Configura el método de filtro
        png_set_filter(png, 0, PNG_FILTER_NONE);

        // Escribir datos del código QR en la imagen PNG con borde blanco
        png_write_info(png, info);
        png_bytep row = new png_byte[png_get_rowbytes(png, info)];

        // Rellena el fondo con blanco
        memset(row, 255, png_get_rowbytes(png, info));

        // Rellena la parte superior con blanco
        for (int i = 0; i < borderSize; ++i) {
            png_write_row(png, row);
        }

        for (int y = 0; y < qr->width; ++y) {
            // Rellena el borde izquierdo con blanco
            memset(row, 255, borderSize * 4);
            for (int x = 0; x < qr->width; ++x) {
                // Píxeles del código QR
                png_bytep pixel = &row[(borderSize + x) * 4];
                pixel[0] = pixel[1] = pixel[2] = (qr->data[y * qr->width + x] & 1) ? 0 : 255;
                pixel[3] = 255;
            }


            // Rellena el borde derecho con blanco
            memset(&row[(borderSize + qr->width) * 4], 255, borderSize * 4);
            png_write_row(png, row);
        }

        // Rellena la parte inferior con blanco
        memset(row, 255, png_get_rowbytes(png, info));
        for (int i = 0; i < borderSize; ++i) {
            png_write_row(png, row);
        }

        // Finaliza la escritura de la imagen
        png_write_end(png, info);
        png_destroy_write_struct(&png, &info);

        // Libera la memoria
        delete[] row;
        return;
    }




    bool Qr_img_t::create(const std::string_view& fname) {
        // Configuración del código QR
        //QRcode* qr = QRcode_encodeString(fname.data(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);                        
        auto qr = std::unique_ptr<QRcode, decltype(&QRcode_free)>(
        QRcode_encodeString(fname.data(), 0, QR_ECLEVEL_L, QR_MODE_8, 1), &QRcode_free);

        // Imprime el código QR en la consola
        #ifdef ENABLE_PRINTS_DBG  
        SET_COLOR(SET_COLOR_WHITE_TEXT);           
        std::cout << "\n";        
        
        for (int y = 0; y < qr->width; y++) {
            for (int x = 0; x < qr->width; x++)   {             
              
               std::cout << (qr->data[y * qr->width + x] & 1 ? "::" : "  ");// std::cout << (qr->data[y * qr->width + x] & 1 ? "██" : "  ");
            }
        std::cout << "\n";
        }
        #endif
        
        codeQrGlobal.height = qr->width;
        codeQrGlobal.width  = qr->width;
        auto value =(codeQrGlobal.width*codeQrGlobal.height);        
        if(!codeQrGlobal.data)codeQrGlobal.data= new bool[value]; 
        if(!codeQrGlobal.bufferComplete)codeQrGlobal.bufferComplete= new uint8_t[value]; 

        for(int i=0;i<value;i++){
            codeQrGlobal.data[i] =(bool*) (qr->data[i]&1 ? true : false);
            codeQrGlobal.bufferComplete[i] = qr->data[i];
        }
        
        // Guarda el código QR como imagen PNG
        const std::string file_tmp = "log/qr_" + tyme->get_tyme()  + ".png";
        
        saveQRCodeImage(qr.get(), file_tmp.c_str());
        // Libera la memoria
        //QRcode_free(qr);//utilizando unique_ptr no es necesario llamar a QRcode_free()
                                
        return true;
    }



    void Qr_img_t::print(){
        std::cout << "\033[" << "15" << ";" << "0" << "H" <<"\n";     
        
        for (int y = 0; y < codeQrGlobal.height; y++) {
            for (int x = 0; x < codeQrGlobal.width; x++)   {             
            //if(codeQrGlobal.data[c++]){std::cout << "::";}
            //else{std::cout << "  ";}
            std::cout << (codeQrGlobal.data[y * codeQrGlobal.width + x] & true ? "::" : "  ");
            // std::cout << (qr->data[y * qr->width + x] & 1 ? "██" : "  ");
            }
        std::cout << "\n";
        }
    }

    Qr_img_t::Qr_img_t()    
    :   tyme { std::make_unique<TYME::Time_t>()}
    {   
        #ifdef DBG
            std::cout<<"Qr_img_t()\n"; 
        #endif 
    }
        
        Qr_img_t::~Qr_img_t(){  
        #ifdef DBG
          //  std::cout<<"DESTROY \t~Qr_img_t()\n";
        #endif 


        }

        void removeQR(){
            delete[] codeQrGlobal.data;
            codeQrGlobal.data = nullptr;
        }

}
