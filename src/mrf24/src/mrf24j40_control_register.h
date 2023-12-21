#pragma once
////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @params Security
/// @author
/// 
////////////////////////////////////////////////////////////////////////////////////////////
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

    typedef struct assoeadr0{
        uint8_t ASSOEADR_7_to_0;
    }ASSOEADR0;

    typedef struct assoeadr1{
        uint8_t ASSOEADR_15_to_8;
    }ASSOEADR1;

    typedef struct assoeadr2{
        uint8_t ASSOEADR_23_to_16;
    }ASSOEADR2;

    typedef struct assoeadr3{
        uint8_t ASSOEADR_31_to_24;
    }ASSOEADR3;

    typedef struct assoeadr4{
        uint8_t ASSOEADR_39_to_32;
    }ASSOEADR4;

    typedef struct assoeadr5{
        uint8_t ASSOEADR_47_to_40;
    }ASSOEADR5;

    typedef struct assoeadr6{
        uint8_t ASSOEADR_55_to_48;
    }ASSOEADR6;

    typedef struct assoeadr7{
        uint8_t ASSOEADR_63_to_56; //<63:56>: 64-Bit Extended Address of Associated Coordinator bits
    }ASSOEADR7;



    typedef struct assoeadr{
        ASSOEADR0 assoeadr0 ;
        ASSOEADR1 assoeadr1 ;
        ASSOEADR2 assoeadr2 ;
        ASSOEADR3 assoeadr3 ;
        ASSOEADR4 assoeadr4 ;
        ASSOEADR5 assoeadr5 ;
        ASSOEADR6 assoeadr6 ;
        ASSOEADR7 assoeadr7 ;     
    }ASSOEADR;



    typedef struct assosadr0{
        uint8_t ASSOSADR_7_to_0; //ASSOSADR<7:0>: 16-Bit Short Address of Associated Coordinator bits
    }ASSOSADR0;

    typedef struct assosadr1{
        uint8_t ASSOSADR_15_to_8; //ASSOSADR<15:8>: 16-Bit Short Address of Associated Coordinator bits
    }
    ASSOSADR1;

    typedef struct assosadr{
        ASSOSADR0 assosadr0;
        ASSOSADR1 assosadr1;
    }ASSOSADR;

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief
    /// @params
    /// @author
    /// 
    ////////////////////////////////////////////////////////////////////////////////////////////
    typedef struct  upnonce0{
        uint8_t upnoce_7_to_0;
    }UPNONCE0;

    typedef struct  upnonce1{
        uint8_t upnoce_15_to_8;
    }UPNONCE1;

    typedef struct  upnonce2{
        uint8_t upnoce_23_to_16;
    }UPNONCE2;

    typedef struct  upnonce3{
        uint8_t upnoce_31_to_24;
    }UPNONCE3;

    typedef struct  upnonce4{
        uint8_t upnoce_39_to_32;
    }UPNONCE4;

    typedef struct  upnonce5{
        uint8_t upnoce_47_to_40;
    }UPNONCE5;

    typedef struct  upnonce6{
        uint8_t upnoce_55_to_48;
    }UPNONCE6;

    typedef struct  upnonce7{
        uint8_t upnoce_63_to_56;
    }UPNONCE7;

    typedef struct  upnonce{
        uint8_t upnoce_71_to_64;
    }UPNONCE8;


    typedef struct  upnonce9{
        uint8_t upnoce_79_to_72;
    }UPNONCE9;

    typedef struct  upnonce10{
        uint8_t upnoce_87_to_80;
    }UPNONCE10;

    typedef struct  upnonce11{
        uint8_t upnoce_95_to_88;
    }UPNONCE11;

    typedef struct  upnonce12{
        uint8_t upnoce_103_to_96;
    }UPNONCE12;

    typedef struct upnonces{ // UPPER NONCE SECURITY  REGISTER 
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

////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @params
/// @author
/// @register TRANSMIT BEACON FIFO CONTROL 0 REGISTER (ADDRESS: 0x1A)
////////////////////////////////////////////////////////////////////////////////////////////

    typedef struct txbcon0{
        uint8_t TXBTRIG:1;      // Transmit Frame in TX Beacon FIFO bit
        uint8_t TXBSECEN:1;     // TX Beacon FIFO Security Enabled bit
                                //1= Securityenabled
                                //0= Securitydisabled(default)
        uint8_t Reserved:6;
    }TXBCON0;// TRANSMIT BEACON FIFO CONTROL 0 REGISTER (ADDRESS: 0x1A)


////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @params
/// @author
/// @register TXNCON: TRANSMIT NORMAL FIFO CONTROL REGISTER (ADDRESS: 0x1B)
////////////////////////////////////////////////////////////////////////////////////////////

    typedef struct txncon{
        uint8_t TXNTRIG:1;          //  1 = Transmit the frame in the TX Normal FIFO; bit is automatically cleared by hardware
        uint8_t TXNSECEN:1;         //  TX Normal FIFO Security Enabled bit(3,4)      
                                    //  1= Securityenabled
        uint8_t TXNACKREQ:1;        //  TX Normal FIFO Acknowledgement Request bit
        uint8_t INDIRECT:1;         //  Activate Indirect Transmission bit (coordinator only)
        uint8_t FPSTAT:1;           //  Frame Pending Status bit
        uint8_t Reserved:3;
    }TXNCON;

}




	


