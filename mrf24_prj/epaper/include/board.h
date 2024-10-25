#pragma once
#include <epaper.h>

namespace EPAPER{

///struct pins_t;


    typedef struct epaperSettingsPins {
        int BS      {22};      //gpio22
        int BUSY    {27};      //gpio27
        
        int RST_N   {17};     //gpio17
        int DC      {18};       //gpio18
        int CSB     {0} ;       //gpio n/open
        int SCL     {3};        //gpio3 //pin 5
        int SDA     {2};        //gpio2 //pin 3
    }configEpaperPin_t;

    

  const pins_t boardRaspberryPi_EXT =
  {
      .panelBusy        =   27, 
      .panelDC          =   18, 
      .panelReset       =   17,
      .panelCS          =   4,
      .panelON_EXT2     =   NOT_CONNECTED,
      .panelSPI43_EXT2  =   NOT_CONNECTED,
      .flashCS          =   0
  };

  const pins_t boardRaspberryPiZero2W =
  {
      .panelBusy        =   25, //red
      .panelDC          =   24, //orange
      .panelReset       =   23,//yelow
      .panelCS          =   27,//gray 
      .panelON_EXT2     =   NOT_CONNECTED,
      .panelSPI43_EXT2  =   NOT_CONNECTED,
      .flashCS          =   22 //violet
  };
    
    const pins_t boardRaspberryPi4 =
    {
        .panelBusy        =   528,    //gpio12 //read
        .panelDC          =   531,    //gpio13
        .panelReset       =   532,    //gpio25
        .panelCS          =   533,    //gpio27
        .panelON_EXT2     =   NOT_CONNECTED,
        .panelSPI43_EXT2  =   NOT_CONNECTED,
        .flashCS          =   538     //gpio22
    };
    
    const pins_t boardRaspberryPiPico_RP2040_EXT2 =
    {
        .panelBusy = 13, ///< EXT3 pin 3 Red -> GP13
        .panelDC = 12, ///< EXT3 pin 4 Orange -> GP12
        .panelReset = 11, ///< EXT3 pin 5 Yellow -> GP11
        .panelCS = 17, //gray
        .panelON_EXT2 = 8,
        .panelSPI43_EXT2 = 7, ///< BS
        .flashCS = 10 //violet
    };

    const pins_t boardRaspberryPi =
        {
            .panelBusy        =   537,//25, //red pin22
            .panelDC          =   536,//24, //orange pin18
            .panelReset       =   535,//23,//yelow pin16 
            .panelCS          =   539,//27,//gray pin13
            .panelON_EXT2     =   NOT_CONNECTED,
            .panelSPI43_EXT2  =   NOT_CONNECTED,
            .flashCS          =   534//22 //violet pin15
            //pin 19 blue gpio10 MOSI
            //pin 21 gren gpio9 MISO
            //pin 23 brow gpio11 sclk
        };

}