#pragma once



#define USE_MRF24_TX
//#define USE_MRF24_RX

#define USE_MAC_ADDRESS_LONG
//#define USE_MAC_ADDRESS_SHORT

#define CHANNEL 24

#ifdef USE_MRF24_TX
    #define MODULE_TX 
    #define MODULE_TX_RST

#else
    #define MODULE_RX 
    #define MODULE_RX_RST
#endif

#ifdef USE_MAC_ADDRESS_SHORT
    #define MSJ "@ABCDEFGHIJKLMKNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz0123456@ABCDEFGHIJKLMKNOPQRS" 
#else
    #define MSJ "@ABCDEFGHIJKLMNO"
#endif