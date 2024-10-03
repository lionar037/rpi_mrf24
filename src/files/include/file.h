#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <string_view>
#include <app/include/work.h>

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

    struct File_t : public WORK::Work_t
    {
        File_t();
        ~File_t();
        unsigned char* loadFile(const std::string_view);
        bool create(const std::string_view&);

        const std::string  tyme();
        
        private :
            const   std::string     m_filename      {};
            const   std::string     m_buffer          {};
            const   size_t          m_size_data       {};
    };

}