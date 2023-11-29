
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


namespace FILESYSTEM{

    File_t::File_t(){

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



        file.close();
    return imgdata;
    }

    void File_t::create(){


    }

}