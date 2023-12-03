#pragma once 

namespace DATA{

    typedef struct MacAdress
    {
        uint8_t ignore[2];
        uint64_t mac;
        uint8_t ignore2;
    }MACADDRESS;


    typedef struct Buffer
    {
    MACADDRESS mac;   
        uint8_t head;
        uint16_t size;
        uint8_t *char;


    }BUFFER;


}