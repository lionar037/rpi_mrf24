//////////////////////////////////////////////////////////////////////////////
//     
//          filename            :   epaper.cpp
//          License             :   GNU 
//          Author              :   Lio
//          Change History      :
//          Processor           :   ARM
//          Hardware            :		
//          Complier            :   ARM
//          Company             :   lionar037
//          Dependencies        :   spi , bcm2835 
//          Description         :   epaper display 
//          @brief              :	
//
//////////////////////////////////////////////////////////////////////////////

#include <others/include/tyme.h>
#include <spi/include/spi.h>
#include <epaper/include/epaper.h>
#include <string>
#include <cstring>


namespace EPAPER{

EPD_Driver::EPD_Driver(uint32_t eScreen_EPD, const pins_t board)
:     Gpio_t            ( enable )
    , spi_ptr           { std::make_unique<SPI::Spi>() }
    , pin_cfg_epaper    { board }
{	
	
	// Type
	pdi_cp      =   (uint16_t) eScreen_EPD;
	pdi_size    =   (uint16_t) (eScreen_EPD >> 8);

	uint16_t screenSizeV = 0;
	uint16_t screenSizeH = 0;
	[[maybe_unused]]uint16_t screenDiagonal = 0;
	[[maybe_unused]]uint16_t refreshTime = 0;
    #ifdef DEBUG
	    std::cout<<"debugger step 0" <<std::endl;
    #endif

    switch (pdi_size)
    {
        case 0x15: // 1.54"

            screenSizeV = 152; // vertical = wide size
            screenSizeH = 152; // horizontal = small size
            screenDiagonal = 154;
            refreshTime = 16;
            break;

        case 0x21: // 2.13"

            screenSizeV = 212; // vertical = wide size
            screenSizeH = 104; // horizontal = small size
            screenDiagonal = 213;
            refreshTime = 15;
            break;

        case 0x26: // 2.66"

            screenSizeV = 296; // vertical = wide size
            screenSizeH = 152; // horizontal = small size
            screenDiagonal = 266;
            refreshTime = 15;
            break;

        case 0x27: // 2.71"

            screenSizeV = 264; // vertical = wide size
            screenSizeH = 176; // horizontal = small size
            screenDiagonal = 271;
            refreshTime = 19;
            break;

        case 0x28: // 2.87"

            screenSizeV = 296; // vertical = wide size
            screenSizeH = 128; // horizontal = small size
            screenDiagonal = 287;
            refreshTime = 14;
            break;

        case 0x37: // 3.70"

            screenSizeV = 416; // vertical = wide size
            screenSizeH = 240; // horizontal = small size
            screenDiagonal = 370;
            refreshTime = 15; // ?
            break;

        case 0x41: // 4.17"

            screenSizeV = 300; // vertical = wide size
            screenSizeH = 400; // horizontal = small size
            screenDiagonal = 417;
            refreshTime = 19;
            break;

        case 0x43: // 4.37"

            screenSizeV = 480; // vertical = wide size
            screenSizeH = 176; // horizontal = small size
            screenDiagonal = 437;
            refreshTime = 21;
            break;

        default:

            break;
    }

    // Force conversion for two unit16_t multiplication into uint32_t.
    // Actually for 1 colour; BWR requires 2 pages.
    image_data_size = (uint32_t) screenSizeV * (uint32_t) (screenSizeH / 8);

	// Config registers according to screen size
	memcpy(register_data, register_data_sm, sizeof(register_data_sm));
}

    int EPD_Driver::digitalRead(int gpio){  // 

        return gpio_get_fd_to_value(gpio);
    }

// CoG initialization function
//		Implements Tcon (COG) power-on and temperature input to COG
//		- INPUT:
//			- none but requires global variables on SPI pinout and config register data
void EPD_Driver::COG_initial()
{	
	pinMode( pin_cfg_epaper.panelBusy, INPUT );     //All Pins 0	
	pinMode( pin_cfg_epaper.panelDC, OUTPUT );

	digitalWrite(pin_cfg_epaper.panelDC, HIGH);
	pinMode( pin_cfg_epaper.panelReset, OUTPUT );
	
    digitalWrite(pin_cfg_epaper.panelReset, HIGH);
	
    pinMode( pin_cfg_epaper.panelCS, OUTPUT );
	digitalWrite(pin_cfg_epaper.panelCS, HIGH);
	if (pin_cfg_epaper.panelON_EXT2 != 0xff){}
	// SPI init	
		 #define SPI_CLOCK_MAX 1600000	
	#if defined(ENERGIA)
	{ }
	#else
    { }
	#endif
	TYME::delay_ms( 5 );
	// Power On COG driver sequence
    reset(5, 5, 10, 5, 5); 
	// reset(1, 5, 10, 5, 1); // en la hoja de datos 

	softReset();
	sendIndexData( 0xe5, &register_data[2], 1 );  //Input Temperature: 25C
	sendIndexData( 0xe0, &register_data[3], 1 );  //Active Temperature
	sendIndexData( 0x00, &register_data[4], 2 );  //PSR
    v_screenSizeV = 296; // vertical = wide size
    v_screenSizeH = 152; // horizontal = small size
}

void EPD_Driver::sendIndexData( uint8_t index, const uint8_t *data, uint32_t len )
    {	
    	digitalWrite( pin_cfg_epaper.panelDC, LOW );      //DC Low
    	digitalWrite( pin_cfg_epaper.panelCS, LOW );      //CS Low
                                                            //delay microseconds
        //spi_ptr->Transfer1bytes( index );
        hV_HAL_SPI_transfer(index);
                                                            //delay microseconds
    	digitalWrite( pin_cfg_epaper.panelCS, HIGH );     //CS High                                                            
    	digitalWrite( pin_cfg_epaper.panelDC, HIGH );     //DC High 
    	digitalWrite( pin_cfg_epaper.panelCS, LOW );      //CS Low // esta pero dentro del spi

        [[maybe_unused]]uint32_t tmp=0;
    	for ( uint32_t i = 0; i < len ; i++ ){
            //spi_ptr->Transfer1bytes( data[ i ] );
            hV_HAL_SPI_transfer(data[ i ]);
            #ifdef DBG_EPAPER
            tmp=i;
            #endif
    	}
        #ifdef DBG_EPAPER
        std::cout<< "len: "<< static_cast<int>(tmp)<< std::endl;
        #endif
    	digitalWrite( pin_cfg_epaper.panelCS, HIGH );     //CS High
    }

    void EPD_Driver::softReset()
    {
    	sendIndexData( 0x00, &register_data[1], 1 );	//Soft-reset, will reset to run the internal LUT for global update       
    	while( digitalRead( pin_cfg_epaper.panelBusy ) != std::stoi(HIGH) );
    }

    void EPD_Driver::reset(uint32_t ms1, uint32_t ms2, uint32_t ms3, uint32_t ms4, uint32_t ms5)
    {
    	// note: group delays into one array
    	TYME::delay_ms(ms1);    /// 5 msec 
        digitalWrite(pin_cfg_epaper.panelReset, HIGH); // RES# = 1
        TYME::delay_ms(ms2);    /// 5 msec 
        digitalWrite(pin_cfg_epaper.panelReset, LOW);
        TYME::delay_ms(ms3);    /// 10 msec 
        digitalWrite(pin_cfg_epaper.panelReset, HIGH);
        // comando reset
        TYME::delay_ms(ms4);    /// 5 msec 
        digitalWrite(pin_cfg_epaper.panelCS, HIGH); // CS# = 1
        TYME::delay_ms(ms5);    /// 5 msec 
    }

    void EPD_Driver::DCDC_powerOn()
    {
    	sendIndexData( 0x04, &register_data[0], 1 );  //Power on
    	while( digitalRead( pin_cfg_epaper.panelBusy ) != std::stoi(HIGH) );
        //{TYME::delay_ms(32);} // en otros codigos deja un delay de 32 milisegundos dentro del while
    }

    // EPD Screen refresh function
    //		- INPUT:
    //			- none but requires global variables on SPI pinout and config register data
    void EPD_Driver::displayRefresh()
    {
    	sendIndexData( 0x12, &register_data[0], 1 );	//Display Refresh
    	while( digitalRead( pin_cfg_epaper.panelBusy ) != std::stoi(HIGH) );
    }

    // Global Update function
    //		Implements global update functionality on either small/mid EPD
    //		- INPUT:
    //			- two image data (either BW and 0x00 or BW and BWR types)
    void EPD_Driver::globalUpdate(const uint8_t * data1s, const uint8_t * data2s)
    {
    	// send first frame

        //image_data_size = (v_screenSizeV * v_screenSizeH )/8;
        sendIndexData(0x10, data1s,image_data_size); // First frame
        //sendIndexData(0x10, data1s, static_cast<uint32_t>(image_data_size)); // First frame
        #ifdef DBG_EPAPER
        std::cout << "cmd 0x10 size : "<< static_cast<int>(size_t(data1s))<<"\n" ;
        #endif
    	// send second frame
    	sendIndexData(0x13, data2s, image_data_size); // Second frame	
        #ifdef DBG_EPAPER
        std::cout << "cmd 0x13 size : "<< static_cast<int>(size_t(data2s))<<"\n" ;
        #endif
    	DCDC_powerOn();
    	displayRefresh();    	
    }

    // CoG shutdown function
    //		Shuts down the CoG and DC/DC circuit after all update functions
    //		- INPUT:
    //			- none but requires global variables on SPI pinout and config register data
    void EPD_Driver::COG_powerOff()
    {
    	sendIndexData( 0x02, &register_data[0], 0 );  //Turn off DC/DC    
    	//while ( digitalRead( spi_basic.panelBusy ) != std::stoi( HIGH)  ); 
               
        while ( digitalRead( pin_cfg_epaper.panelBusy ) != std::stoi(HIGH));
    	digitalWrite( pin_cfg_epaper.panelDC, LOW );
    	digitalWrite( pin_cfg_epaper.panelCS, LOW );
    	digitalWrite( pin_cfg_epaper.panelBusy, LOW );
    	TYME::delay_ms( 150 );
    	digitalWrite( pin_cfg_epaper.panelReset, LOW );
    }

    uint8_t EPD_Driver::hV_HAL_SPI_transfer( uint8_t command){
        return spi_ptr->Transfer1bytes(command);
    }

}