#pragma once
#include <stdint.h>

namespace MRF24J40{

enum ENUM_SECURITY{
NONE                ,
AES_CTR             ,
AES_CCM_128         ,
AES_CCM_64          ,
AES_CCM_32          ,
AES_CBC_MAC_128     ,
AES_CBC_MAC_64      ,
AES_CBC_MAC_32 
};

    typedef struct seccr0{
        uint8_t TXNCIPHER:3;
        uint8_t RXCIPHER:3;
        uint8_t SECSTART:1; //RX Security Decryption Start bit
        uint8_t SECIGNORE:1;//RX Security Decryption Ignore bit
    }SECCR0 ;

    typedef struct seccr1{
        uint8_t DISENC      :1;       //Disable Encryption Function bit
        uint8_t DISDEC      :1;       //Disable Decryption Function bit
        uint8_t Reserved0   :2;
        uint8_t TXBCIPHER   :3;
        uint8_t Reserved    :1;
    }SECCR1 ;

    typedef struct seccr2{
        uint8_t TXG1CIPHER       :3;    // TX GTS1 FIFO Security Suite Select bits 
                                        // 111 = AES-CBC-MAC-32
                                        // 110 = AES-CBC-MAC-64
                                        // 101 = AES-CBC-MAC-128
                                        // 100 = AES-CCM-32 
                                        // 011 = AES-CCM-64 
                                        // 010 = AES-CCM-128 
                                        // 001 = AES-CTR
                                        // 000 = None (default)
                                
        uint8_t TXG2CIPHER      :3;     // TX GTS2 FIFO Security Suite Select bits   
                                        // 111 = AES-CBC-MAC-32 
                                        // 110 = AES-CBC-MAC-64 
                                        // 101 = AES-CBC-MAC-128 
                                        // 100 = AES-CCM-32
                                        // 011 = AES-CCM-64 
                                        // 010 = AES-CCM-128 
                                        // 001 = AES-CTR
                                        // 000 = None (default)        
        uint8_t UPENC       :1; //Upper Layer Security Encryption Mode bit
                                

        uint8_t UPDEC    :1; //Upper Layer Security Decryption Mode bit


    }SECCR2;                // SECURITY CONTROL 2 REGISTER (ADDRESS: 0x37)

    typedef struct  seccr
    {
     SECCR0 seccr0;
     SECCR1 seccr1;
     SECCR2 seccr2;   
    }SECCR;


// Alinear la estructura CODE en 3 bytes
typedef struct alignas(1) {
    SECCR seccr;
} ALIGNED_SECCR;

}