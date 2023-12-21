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

    typedef struct seccon0{
        uint8_t TXNCIPHER:3;
        uint8_t RXCIPHER:3;
        uint8_t SECSTART:1; //RX Security Decryption Start bit
        uint8_t SECIGNORE:1;//RX Security Decryption Ignore bit
    }SECCON0 ;

    typedef struct seccon1{
        uint8_t DISENC      :1;       //Disable Encryption Function bit
        uint8_t DISDEC      :1;       //Disable Decryption Function bit
        uint8_t Reserved0   :2;
        uint8_t TXBCIPHER   :3; // TX Beacon FIFO Security Suite Select bits
        uint8_t Reserved    :1;
    }SECCON1 ;

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
        SECCON0 seccon0;
        SECCON1 seccon1;
        SECCR2 seccr2;   
    }SECCR;


    // Alinear la estructura CODE en 3 bytes
    typedef struct alignas(1) {
        SECCR seccr;
    } ALIGNED_SECCR;



    typedef struct  upnonce0{
        uint8_t upnoce;
    }UPNONCE0;

    typedef struct  upnonce1{
        uint8_t upnoce;
    }UPNONCE1;

    typedef struct  upnonce2{
        uint8_t upnoce;
    }UPNONCE2;

    typedef struct  upnonce3{
        uint8_t upnoce;
    }UPNONCE3;

    typedef struct  upnonce4{
        uint8_t upnoce;
    }UPNONCE4;

    typedef struct  upnonce5{
        uint8_t upnoce;
    }UPNONCE5;

    typedef struct  upnonce6{
        uint8_t upnoce;
    }UPNONCE6;

    typedef struct  upnonce7{
        uint8_t upnoce;
    }UPNONCE7;

    typedef struct  upnonce{
        uint8_t upnoce;
    }UPNONCE8;


    typedef struct  upnonce9{
        uint8_t upnoce;
    }UPNONCE9;

    typedef struct  upnonce10{
        uint8_t upnoce;
    }UPNONCE10;

    typedef struct  upnonce11{
        uint8_t upnoce;
    }UPNONCE11;

    typedef struct  upnonce12{
        uint8_t upnoce;
    }UPNONCE12;

    typedef struct upnonces{
         UPNONCE0 upnonce0 ;
         UPNONCE1 upnonce1 ;
         UPNONCE2 upnonce2 ;
         UPNONCE3 upnonce3 ;
         UPNONCE4 upnonce4 ;
         UPNONCE5 upnonce5 ;
         UPNONCE6 upnonce6 ;
         UPNONCE7 upnonce7 ;
         UPNONCE8 upnonce8 ;
         UPNONCE9 upnonce9 ;
        UPNONCE10 upnonce10 ;
        UPNONCE11 upnonce11 ;
        UPNONCE12 upnonce12 ; 
    }UPNONCES;
}