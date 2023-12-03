#pragma once 

namespace DATA{

    typedef struct MacAdress
    {
        //uint8_t ignore[3];

        uint8_t ignore;
    }MACADDRESS;


    typedef struct Buffer
    {
    //MACADDRESS ignore;   
        uint8_t mac[11]{0x00};
        uint8_t head{0x00};
        uint16_t size{0x00};
        uint8_t data[107]{0x00};

    }BUFFER;


}