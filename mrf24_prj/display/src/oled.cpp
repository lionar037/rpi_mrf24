#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <display/include/oled.h>
#ifdef USE_MRF24_RX
        #include <SSD1306_OLED.hpp>
#endif


//#include <memory>
//#include <vector>
//#include <algorithm>

//#define OLED

//GLOBAL namespace OLED
namespace OLED{
#ifdef USE_MRF24_RX

SSD1306 myOLED(myOLEDwidth , myOLEDheight) ; // instantiate  an object         
static int count { 0 };

void Oled_t::function(){

	myOLED.setTextWrap(true);
	myOLED.setFontNum(OLEDFont_Default);

	myOLED.OLEDclearBuffer(); // Clear the buffer
	// Test 701
	printf("OLED Test 701 Default Font size 3 float\r\n");
	myOLED.setTextColor(WHITE);
	myOLED.setTextSize(3);
	myOLED.setCursor(0, 0);
	float PI = 3.14;
	myOLED.print(PI);

	// Test 702
	printf("OLED Test 702 Default font size 2 integer\r\n");
	myOLED.setTextSize(2);
	myOLED.setCursor(0, 30);
	int count = 123;
	myOLED.print(count);

	// Test 703
	printf("OLED Test 703 Default font, size 1 string inverted\r\n");
	myOLED.setTextSize(1);
	myOLED.setTextColor(BLACK, WHITE);
	myOLED.setCursor(0, 50);
	myOLED.print("Hello World");

	// Test 704
	printf("OLED Test 704 Default font ,size 4, single character\r\n");
	myOLED.drawChar(95, 15 , 'H', WHITE, BLACK, 6);
}

bool Oled_t::create(const std::string_view& textOnOled){
        #ifdef OLED
        
        static int Step { 0 };        
        uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight/8)+1]; 
                
                // Define a buffer to cover whole screen                 
                myOLED.buffer = (uint8_t*) &screenBuffer;  // set that to library buffer pointer
                myOLED.OLEDclearBuffer();  
            	myOLED.setTextSize(1);
                myOLED.setFontNum(OLEDFontType_Wide);
                myOLED.setTextColor(WHITE);
                myOLED.setCursor(0, 0);
                myOLED.print(textOnOled.data());
                myOLED.setFontNum(OLEDFontType_Default);
                                
                myOLED.setCursor(128-24, 64-9);
                myOLED.print(reinterpret_cast<const int>(Step));
                myOLED.OLEDupdate();
                Step++;
                #else
                function();
                #endif
            return true;
        }

        bool Oled_t::init(){
                    if(!bcm2835_init())
                {
                        printf("Error 1201: init bcm2835 library\r\n");
                        return -1;
                }

                Setup();
        return true;
        }


        void Oled_t::printOled(std::string_view msj){
                static std::string msjStatic { msj };
                if(msj.data()!= msjStatic.c_str()){
                        create(msj);
                        msjStatic = msj.data();
                }
                
                return;
        }

        // ===================== Function Space =====================
        void Oled_t::Setup() 
        {
                bcm2835_delay(100);
                myOLED.OLEDbegin(I2C_Speed, I2C_Address); // initialize the OLED
                myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
                bcm2835_delay(10);
                myOLED.OLEDclearBuffer();
                return;
        }

        void Oled_t::End()
        {
                myOLED.OLEDPowerDown(); //Switch off display
                bcm2835_close(); // Close the library
        return;
        }

void Oled_t::test(){
        	myOLED.setRotation(OLED_Degrees_0);
	// Define a buffer to cover whole screen 
	uint8_t  screenBuffer[FULLSCREEN]; 
	if (!myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer))) return;
	myOLED.OLEDclearBuffer(); // clear the buffer

	// Set text parameters
	myOLED.setTextColor(WHITE);
	myOLED.setTextSize(1);


	//  ** Test 501 OLED display enable and disable **
	myOLED.setCursor(0, 30);
	myOLED.print("Disable test 501");
	printf("OLED Disable test 501\r\n");
	myOLED.OLEDupdate();

	bcm2835_delay(2000); 
	myOLED.OLEDEnable(0); 
	bcm2835_delay(2000); 
	myOLED.OLEDEnable(1); 
	bcm2835_delay(2000); 
	myOLED.OLEDclearBuffer();

	// ** Test 502 inverse **
	myOLED.setCursor(0, 10);
	myOLED.print("Inverse test 502");
	printf("OLED Inverse test 502\r\n");
	myOLED.OLEDupdate();
	bcm2835_delay(1500);
	myOLED.OLEDInvert(1); // Inverted
	bcm2835_delay(1500);
	myOLED.OLEDInvert(0);
	bcm2835_delay(1500);

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
	myOLED.setCursor(20,20 );
	myOLED.print("Scroll test 504");
	printf("OLED Scroll test 504\r\n");
	myOLED.OLEDupdate();
	bcm2835_delay(2500);

	myOLED.OLEDStartScrollRight(0, 0x0F);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();
	
	myOLED.OLEDStartScrollLeft(0, 0x0F);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();

	myOLED.OLEDStartScrollDiagRight(0, 0x07);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();
 	
	myOLED.OLEDStartScrollDiagLeft(0, 0x07);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();
 	
 	
 	// ** Test 505 rotate test **
 	
 	myOLED.OLEDclearBuffer();
 	printf("OLED Rotate test 505\r\n");
	myOLED.setRotation(OLED_Degrees_90);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 90");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	myOLED.setRotation(OLED_Degrees_180);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 180");
	myOLED.setCursor(5,50);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	
	myOLED.setRotation(OLED_Degrees_270);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate   270");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	myOLED.setRotation(OLED_Degrees_0); //default normal 
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 0");
	myOLED.setCursor(5,50);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	// ** end of tests **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("End of tests");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
}

        void Oled_t::Start()
        {
                
                // Define a buffer to cover whole screen 
                static uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight/8)+1]; 
                #ifdef OLED
                        myOLED.buffer = (uint8_t*) &screenBuffer;  // set that to library buffer pointer
                
                        myOLED.OLEDclearBuffer();  
            	        myOLED.setTextSize(1);
                	//myOLED.setFontNum(OLEDFontType_Thick);
                        //myOLED.setFontNum(OLEDFontType_SevenSeg);
                        //myOLED.setFontNum(OLEDFontType_Wide);
                        //myOLED.setFontNum(OLEDFontType_Tiny);
                        //myOLED.setFontNum(OLEDFontType_Homespun);
                        myOLED.setFontNum(OLEDFontType_Default);
                	//myOLED.OLEDInvert(1); // Inverted
                        myOLED.setTextColor(WHITE);
                        myOLED.setCursor(12, 32);

                        myOLED.print("MRF24J40");
                        //myOLED.setCursor(36, 14);
                        //printf(" count : %d\n",count);
                        //myOLED.print(reinterpret_cast<int>(count));
                        myOLED.OLEDupdate();
                        count++;
                        #else
                        test();
                        #endif

            
    }

void  Oled_t::DisplayGraphics(){
        uint8_t graphicsCountLimit = 25;
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	printf("OLED Test 901 graphics test \r\n");
	bool colour = 1;
	uint8_t count = 0;
	myOLED.setFontNum(OLEDFont_Default);
	myOLED.setTextSize(1);
	myOLED.OLEDclearBuffer(); // Clear the buffer
	while (count < graphicsCountLimit)
	{
		colour = !colour;

		//print count
		myOLED.setCursor(0, 0);
		myOLED.print(count);
		myOLED.setCursor(45, 0);
		myOLED.print(colour);

		// Draw the X
		myOLED.drawLine(64,  0, 64, 64, WHITE);
		myOLED.drawFastVLine(62, 0, 64, WHITE);
		myOLED.drawFastHLine(0, 32, 128, WHITE);

		// Q1
		myOLED.fillRect(0, 10, 20, 20, colour);
		myOLED.fillCircle(40, 20, 10, WHITE);

		// Q2
		myOLED.fillTriangle(80, 25, 90, 5, 100, 25, !colour);
		myOLED.drawRect(105, 10, 15, 15, WHITE);
		// Q3
		myOLED.fillRoundRect(0, 40, 40, 20, 10, !colour);
		// Q4
		char i;
		for (i = 0; i < 10; i ++)
		{
			myOLED.drawRect(70 + i, 40 + i, 50 - i * 2, 20 - i * 2, WHITE);
			myOLED.OLEDupdate();
			bcm2835_delay(50);
		}
		myOLED.OLEDclearBuffer();
		count++;
	}
}
        void  Oled_t::Graphics(const int x,const int y,const bool* z,const uint8_t* w){
                
                #ifdef OLED
                uint8_t buff [(x+3)*(y+3)]={0x00};
                int l{-1},Position{0};
                //int module =0;

                //std::cout << "\033[" << "15" << ";" << "0" << "H" <<"\n";  

                //for(int i=0 ; i< (y+3)*(x+3); i++){buff[l++]=0x00;}
                l=-1;

                for( int i=0 ; i<(y)*(x) ; )
                {                
                        if((!(i % 29) ) || Position==0)                
                        {
                                l++;
                                Position=8;                                                                      
                        }
                        Position--; 
                        //if(i<(x*y)) 
                        buff[l] |= (w[i] & true ? 1 : 0) << Position ;                                                                                    
                        i++;                                                                                
                }        
                uint8_t fullscreenBuffer[1024]; 
                myOLED.buffer = (uint8_t*) &fullscreenBuffer; // buffer to the pointer
                myOLED.OLEDclearBuffer();  
                static int move{0};
                
                if(move==32)move=0;

                myOLED.OLEDBitmap(move++, 0 , x, y, buff, false);                
                myOLED.setCursor(128-24, 64-12);
                myOLED.setFontNum(OLEDFontType_Wide);
                myOLED.print(reinterpret_cast<int>(count++));                
                myOLED.OLEDupdate();
                #else
                DisplayGraphics();
                #endif
        }



    Oled_t::Oled_t(){
        init();        
    }

    Oled_t::~Oled_t(){
        End();
    }
#endif
}