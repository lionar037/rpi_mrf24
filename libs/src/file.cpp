
#include <iostream>
#include <fstream>
extern "C"
{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdint.h>
}

#include <file.h>
#include <config.h>
#include <color.h>



namespace FILESYSTEM{

    File_t::File_t()
    : m_filename {"datos.bin"} , m_buffer{"@ABCDEF"} , m_size_data{m_buffer.size() * sizeof(char)}
    {

    }

    File_t::~File_t(){

    }


    unsigned char* File_t::loadFile(const char* filename){

            std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return nullptr;
    }

    packet_mrf24 packet;
    
    file.read(reinterpret_cast<char*>(&packet), sizeof(packet_mrf24));

    const auto& address_rx = packet.mac_address_rx;

    if (address_rx != ADDRESS_LONG_SLAVE) {

        std::cerr << "Es un MRF24 no  válido." << std::endl;
        file.close();
        return nullptr;
    }

size_t dataSize = packet.size;

    unsigned char* imgdata = new unsigned char[dataSize];

    //file.seekg(packet.data, std::ios::binary);// or std::ios::beg 

    file.read(reinterpret_cast<char*>(imgdata), dataSize);

    if (packet.panid != PAN_ID) {

        std::cerr << SET_COLOR_RED_TEXT << "PAN_ID no  válido." << std::endl;
        file.close();
        return nullptr;
    }

        file.close();
    return imgdata;
    }


    bool File_t::create(const char * tmp){
        std::ofstream file(m_filename, std::ios::binary);
        if (file.is_open()) {
        // Escribir los datos en el archivo
            //file.write(reinterpret_cast<char*>(buffer), size_data);
           // file.write(m_buffer.c_str(), m_size_data);
           file.write ( tmp , m_size_data);
            // Cerrar el archivo
            file.close();

            std::cout << "Datos escritos en el archivo correctamente." << std::endl;
                  return true;
        } else {
            std::cerr << "No se pudo abrir el archivo." << std::endl;
                  return false;
        }

        return false;

    }

}