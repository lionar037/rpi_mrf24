#pragma once

#if defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4)
    // Si es de 32 bits
    #define USE_MRF24_RX
#else
    // Si es de 64 bits
    #define USE_MRF24_TX
#endif

//#define USE_MAC_ADDRESS_LONG
#define USE_MAC_ADDRESS_SHORT

//Enable database 
//#define ENABLE_DATABASE

#define ENABLE_MOSQUITTO

#define CHANNEL 24
#define HEAD 0x40 //inicial MSJ

#ifdef USE_MRF24_TX
    #define MODULE_IS_TX 
    #define MODULE_TX_RST
#else
    #define MODULE_IS_RX 
    #define MODULE_RX_RST
#endif

#ifdef USE_MAC_ADDRESS_SHORT
    //#define MSJ "ABCDEFGHIJKLMKNOFGHIJKLMKNOPQRS@ABCDEFGHIJKLMKNOPQRS@ABCDEFGHIJKLMKNOPQRS" 
    #define MSJ "ABCDEFGHIJKLMKNOFGHIJKLMKNOPQRSTUVWXYZ0MNO_____________"
#else 
    #define MSJ "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmno_____________"
    //#define MSJ "ABCDEFGHIJKLMKNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz0123456@ABCDEFGHIJKLMKNOPQRS@ABCDEFGHIJKLMKNOPQ" 
#endif

#ifdef MODULE_IS_TX
    #define ADDRESS_LONG 0x1122334455667702
    #define ADDRESS_LONG_SLAVE 0x1122334455667701
    #define ADDRESS 0x6001
    #define PAN_ID 0x1234
    #define ADDRESS_SLAVE 0x6002
    #define MRF24_TRANSMITER_ENABLE
    #define MRF24_RECEIVER_ENABLE
//#endif    
#elif  defined(MODULE_IS_RX)
    #define ADDRESS_LONG 0x1122334455667701
    #define ADDRESS_LONG_SLAVE 0x1122334455667702
    #define ADDRESS 0x6002
    #define PAN_ID 0x1235
    #define ADDRESS_SLAVE  0x6001
    #define MRF24_RECEIVER_ENABLE
    #define MRF24_TRANSMITER_ENABLE
    #define ENABLE_INTERRUPT_MRF24
#endif

#define LOG_FILENAME "log_mrf_"

// Prints Debugger

//#define DBG
//#define DBG_BUFFER
//#define DBG_FILES //imprime Debugger en files
//#define DBG_DISPLAY_OLED
//#define DBG_MRF
//#define DBG_RADIO
//#define ENABLE_PRINTS_DBG
//#define DBG_PRINT_GET_INFO
//#define DBG_GPIO
//#define DBG_OLED


//config QR string 
//Linea de configuracion para codigo Qr de una red wifi
//#define QR_CODE_SRT "WIFI:T:WPA;S:MiRedWiFi;P:MiContraseña123;;";
#define QR_CODE_SRT MSJ   

#ifdef ENABLE_MOSQUITTO
    //#define HOST_SERVER_MOSQUITTO "mosquitto.local"
    //#define HOST_SERVER_MOSQUITTO "raspberrypi.local"
    #define HOST_SERVER_MOSQUITTO "raspberry.local"
    //#define HOST_SERVER_MOSQUITTO "rpi2w.local"
#endif

#define QR_START

#ifdef QR_START
    #define ENABLE_QR    
#endif

#ifdef USE_MRF24_TX
//habilita la encriptacion de mensajes 
    //#define ENABLE_SECURITY 
#endif
#ifdef USE_MRF24_RX
    //#if defined(__LP64__) || defined(_LP64) || defined(__LLP64__) 
    #if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4))//es de 32 bits?
        #define USE_OLED2
    #endif    
#else
#endif
