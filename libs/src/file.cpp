
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


    unsigned char* loadFile(const char* filename){

            std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return nullptr;
    }

    packet_mrf24 packet;
    
    file.read(reinterpret_cast<char*>(&packet), sizeof(packet_mrf24));


    const auto& mac_address_rx = packet.head;

    if (mac_address_rx != ADDRESS_LONG_SLAVE) {

        std::cerr << "Es un MRF24 no  válido." << std::endl;
        file.close();
        return nullptr;
    }



        file.close();
    return 0;
    }

}