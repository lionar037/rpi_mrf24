#pragma once
#include <iostream>
#include <string>

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
        bool create(const char*);

        uint64_t tyme();
        
        private :
            const   std::string     m_filename      {};
            const   std::string     m_buffer          {};
            const   size_t          m_size_data       {};
    };

}