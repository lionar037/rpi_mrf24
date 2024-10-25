////////////////////////////////////////////////////////////////////////////                
//          
//      
//                oled.cpp
//
//
////////////////////////////////////////////////////////////////////////////

#include <display/include/oled.h>
#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
//#include <cstdio>
//#include <string>
//#ifdef USE_MRF24_RX
//	#include <SSD1306_OLED.hpp>
//#endif


namespace OLED{

    Oled_t::Oled_t(uint16_t width, uint16_t height, uint16_t i2c_speed, uint8_t i2c_address)
        : 
		myOLEDwidth(width), 
		myOLEDheight(height), 
		i2c_speed_(i2c_speed), 
		i2c_address_(i2c_address), 
		i2c_debug_(false), 
		screenBuffer_(nullptr),
		myOLED(width, height) // Inicializar el objeto SSD1306 aquí
    {}

    Oled_t::~Oled_t() {
        powerDown();
    }

    bool Oled_t::begin(bool i2c_debug) {
        i2c_debug_ = i2c_debug;
        
        if (!bcm2835_init()) {
            printf("Error: init bcm2835 library failed\n");
            return false;
        }

        if (!initI2C()) {
            bcm2835_close();
            return false;
        }

        myOLED.OLEDbegin(i2c_speed_, i2c_address_, i2c_debug_);
        screenBuffer_ = new uint8_t[FULLSCREEN];
        myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer_, FULLSCREEN);
        //myOLED.OLEDFillScreen(0xF0, 0);
        bcm2835_delay(100);

        return true;
    }


    void Oled_t::clearScreen() {
        myOLED.OLEDclearBuffer();
    }

    void Oled_t::displayTextScroll(const char* text, int x, int y) {
        //myOLED.setFontNum(OLEDFont_Mednum);
        myOLED.setFontNum(OLEDFont_Default);
        myOLED.setTextColor(BLACK, WHITE); 
        myOLED.setTextColor(WHITE);
        myOLED.setCursor(x, y);
        myOLED.print(text);
        myOLED.OLEDupdate();		
		myOLED.OLEDStartScrollLeft (0, 0x0F);		
		bcm2835_delay(3000);
		myOLED.OLEDStopScroll();
    }


    void Oled_t::displayText(const char* text, int x, int y) {        
        myOLED.setFontNum(OLEDFont_Default);
        myOLED.setTextColor(BLACK, WHITE); 
        myOLED.setTextColor(WHITE);
        myOLED.setCursor(x, y);
        myOLED.print(text);
        myOLED.OLEDupdate();							
    }


    void Oled_t::powerDown() {
        myOLED.OLEDPowerDown();
        closeI2C();
        bcm2835_close();
        delete[] screenBuffer_;
    }

    bool Oled_t::initI2C() {
        if (!myOLED.OLED_I2C_ON()) {
            printf("Error: Cannot start I2C\n");
            return false;
        }
        return true;
    }

    void Oled_t::closeI2C() {
        myOLED.OLED_I2C_OFF();
    }

    void Oled_t::demo(){
    //bool colour = 1;
    //myOLED.fillRect(0, 10, 20, 20, colour);
    //myOLED.fillCircle(40, 20, 10, WHITE);
    //// Q2
    //myOLED.fillTriangle(80, 25, 90, 5, 100, 25, !colour);
    //myOLED.drawRect(105, 10, 15, 15, WHITE);
    //// Q3
    //myOLED.fillRoundRect(0, 40, 40, 20, 10, !colour);
    //myOLED.drawRect(70 + i, 40 + i, 50 - i * 2, 20 - i * 2, WHITE);
    //myOLED.OLEDupdate();
	//myOLED.setFontNum(OLEDFont_ArialRound);
	myOLED.setRotation(OLED_Degrees_0);
	// Define a buffer to cover whole screen 
	uint8_t  screenBuffer[FULLSCREEN]; 
	if (!myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer))) return;
	myOLED.OLEDclearBuffer(); // clear the buffer

	// Set text parameters
	myOLED.setTextColor(BLACK);
	//myOLED.setTextSize(1);

	//  ** Test 501 OLED display enable and disable **
	myOLED.setCursor(0, 0);
	myOLED.print("Disable test 501");
	printf("OLED Disable test 501\r\n");
	myOLED.OLEDupdate();

	bcm2835_delay(900); 
	myOLED.OLEDEnable(0); 
	bcm2835_delay(900); 
	myOLED.OLEDEnable(1); 
	bcm2835_delay(900); 
	myOLED.OLEDclearBuffer();

	// ** Test 502 inverse **
	myOLED.setCursor(0, 0);
	myOLED.print("Inverse test 502");
	printf("OLED Inverse test 502\r\n");
	myOLED.OLEDupdate();
	bcm2835_delay(900);
	myOLED.OLEDInvert(1); // Inverted
	bcm2835_delay(900);
	myOLED.OLEDInvert(0);
	bcm2835_delay(900);

	// ** Test 503 contrast **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("Contrast test 503");
	printf("OLED Contrast test 503\r\n");
	myOLED.OLEDupdate();
	bcm2835_delay(1500);
	myOLED.OLEDFillScreen(0x77, 0); 
	myOLED.OLEDContrast(0x00);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(0x80);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(0xFF);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(0x81);
	bcm2835_delay(1000);
	myOLED.OLEDclearBuffer();

	// ***** Test 504 Scroll **

	myOLED.setCursor(16,16 );
	myOLED.print("SCROLL EST 504  ");
	printf("OLED Scroll test 504\r\n");
	myOLED.OLEDupdate();

	bcm2835_delay(2500);
	myOLED.OLEDStartScrollRight(0, 0x0F);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();
	printf("OLEDStartScrollRight\r\n");
	
	myOLED.OLEDStartScrollLeft(0, 0x0F);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();
	printf("OLEDStartScrollLeft\r\n");

	myOLED.OLEDStartScrollDiagRight(0, 0x07);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();
	printf("OLEDStartScrollDiagRight\r\n");
 	
	myOLED.OLEDStartScrollDiagLeft(0, 0x07);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();
	printf("OLEDStartScrollDiagLeft\r\n"); 	 	

    }

    const std::vector <std::string>& Oled_t::convertToMayuscule( std::vector <std::string>& display_text ){
		
        //std::transform(display_text.begin(), display_text.end(), display_text.begin(), ::toupper); 
		for (auto& str : display_text) 
        // Convertir cada cadena a mayúsculas
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
		
    return display_text;    
    }


}