#pragma once
#include <stdint.h>

namespace MRF24J40{


    
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
}