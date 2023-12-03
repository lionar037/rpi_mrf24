#pragma once 

namespace DATA{
#pragma pack(push, 1)
    typedef struct MacAdress
    {
        //uint8_t ignore[3];

        uint8_t ignore;
    }MACADDRESS;


    typedef struct buffer_rx
    {
        uint32_t mac_msb;
        uint32_t mac_lsb;
        uint8_t ignore[4];
        uint8_t head;
        uint16_t size;
        uint8_t data[107];
        
    }BUFFER;

        typedef struct buffer_tx
    {
        uint8_t head;
        uint16_t size;
        const char* data;
        
    }BUFFER_TX;

#pragma pack(pop)
}