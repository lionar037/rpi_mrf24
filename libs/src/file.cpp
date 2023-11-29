
#include <iostream>
#include <fstream>
extern "C"
{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdint.h>
}

#include <file.h>


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

    file.read(reinterpre t_cast<char*>(&packet), sizeof(packet_mrf24));


        return 0;
    }

}