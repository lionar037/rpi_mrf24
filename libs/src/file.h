#pragma once
#include <iostream>
#

namespace FILESYSTEM{


#pragma pack(push, 1)

    struct packet_mrf24 {
        uint32_t ignore;    
        uint64_t mac_address_rx;
        uint16_t size;
        uint16_t panid;
        uint8_t* data;
        uint16_t checksum;
    };

#pragma pack(pop)

    struct File_t
    {
        File_t();
        ~File_t();
        unsigned char* loadFile(const char*);
        void create();


        private :
          const char* nombreArchivo {};
          const char buffer[128]{};
            const size_t tamanoDatos{};
    };

}