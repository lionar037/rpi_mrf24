#pragma once 

namespace DATA{
#pragma pack(push, 1)
    typedef struct MacAdress
    {
        //uint8_t ignore[3];

        uint8_t ignore;
    }MACADDRESS;


    typedef struct packet_rx
    {
        uint32_t mac_msb;
        uint32_t mac_lsb;
        uint8_t ignore[4];
        uint8_t head;
        uint16_t size;
        uint8_t data[107];
        uint16_t checksum;
        
    }PACKET_RX;


    typedef struct packet_tx
        {
            uint8_t head;
            uint16_t size;
            char data[107];
            uint16_t checksum;
        }PACKET_TX;


    typedef struct qr_oled{
            uint8_t width;
            uint8_t height;
            uint8_t data[(64*64/8)];
    }QR_OLED;

#pragma pack(pop)
}