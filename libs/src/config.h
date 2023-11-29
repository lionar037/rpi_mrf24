#pragma once



#define USE_MRF24_TX
//#define USE_MRF24_RX

#define USE_MAC_ADDRESS_LONG
//#define USE_MAC_ADDRESS_SHORT

//#define ENABLE_DATABASE

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
    //#define MSJ "@ABCDEFGHIJKLMNO"
      #define MSJ "@ABCDEFGHIJKLMKNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz0123456@ABCDEFGHIJKLMKNOPQRS" 
#endif




#ifdef MODULE_TX
    #define ADDRESS_LONG 0x1122334455667702
    #define ADDRESS_LONG_SLAVE 0x1122334455667701
    #define ADDRESS 0x6001
    #define PAN_ID 0x1234
    #define ADDR_SLAVE 0x6002
    #define MRF24_TRANSMITER_ENABLE
//#endif    
#elif  defined(MODULE_RX)
    #define ADDRESS_LONG 0x1122334455667701
    #define ADDRESS_LONG_SLAVE 0x1122334455667702
    #define ADDRESS 0x6002
    #define PAN_ID 0x1235
    #define ADDR_SLAVE  0x6001
    #define MRF24_RECEIVER_ENABLE
    #define ENABLE_INTERRUPT_MRF24
    
#endif