#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include <oled/oled.h>
#ifdef USE_MRF24_RX
    #include <SSD1306_OLED.hpp>
#else
#endif

namespace OLED{
#ifdef USE_MRF24_RX

SSD1306 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object 
        bool Oled_t::create(const char* tmp){
                static int count { 0 };
            // Define a buffer to cover whole screen 
            uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight/8)+1]; 
            myOLED.buffer = (uint8_t*) &screenBuffer;  // set that to library buffer pointer
            myOLED.OLEDclearBuffer();  
            	myOLED.setTextSize(1);
                        myOLED.setFontNum(OLEDFontType_Wide);
                        myOLED.setTextColor(WHITE);
                        myOLED.setCursor(0, 0);

                        myOLED.print(tmp);
                        myOLED.setFontNum(OLEDFontType_Default);
                        myOLED.setCursor(128-24, 64-9);
                        myOLED.print(reinterpret_cast<int>(count));
                        myOLED.OLEDupdate();
                        count++;
            return true;
        }

        bool Oled_t::init(){
                    if(!bcm2835_init())
                {
                        printf("Error 1201: init bcm2835 library\r\n");
                        return -1;
                }

                Setup();
                //TestLoop();
                //EndTest();

        return true;
        }


        // ===================== Function Space =====================
void Oled_t::Setup() 
{
        bcm2835_delay(100);
        #ifdef DBG_DISPLAY_OLED
        printf("\nOLED Begin\r\n");
        #endif
        myOLED.OLEDbegin(I2C_Speed, I2C_Address); // initialize the OLED
myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
        bcm2835_delay(100);
//myOLED.OLEDclearBuffer();
        return;
}

    void Oled_t::End()
    {
            myOLED.OLEDPowerDown(); //Switch off display
            bcm2835_close(); // Close the library
            printf("OLED End\r\n");
        return;
    }



        void Oled_t::Start()
        {
                static int count { 0 };
            // Define a buffer to cover whole screen 
            uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight/8)+1]; 
                        myOLED.buffer = (uint8_t*) &screenBuffer;  // set that to library buffer pointer
                
                        myOLED.OLEDclearBuffer();  
            	        myOLED.setTextSize(1);
                	//myOLED.setFontNum(OLEDFontType_Thick);
                        //myOLED.setFontNum(OLEDFontType_SevenSeg);
                        myOLED.setFontNum(OLEDFontType_Wide);
                        //myOLED.setFontNum(OLEDFontType_Tiny);
                        //myOLED.setFontNum(OLEDFontType_Homespun);
                        //myOLED.setFontNum(OLEDFontType_Default);
                	//myOLED.OLEDInvert(1); // Inverted
                        myOLED.setTextColor(WHITE);
                        myOLED.setCursor(0, 0);

                        myOLED.print("MRF24J40");
                        myOLED.setCursor(36, 14);
                        printf(" count : %d\n",count);
                        myOLED.print(reinterpret_cast<int>(count));
                        myOLED.OLEDupdate();
                        count++;
                        //myOLED.setRotation(3);

                        // myOLED.OLEDContrast(0x00);
                        // bcm2835_delay(1000);
                        // myOLED.OLEDContrast(0x80);
                        // bcm2835_delay(1000);
                        // myOLED.OLEDContrast(0xFF);
                        // bcm2835_delay(1000);
                        // myOLED.OLEDContrast(0x81);
            
            return;
    }
    

    Oled_t::Oled_t(){
        init();
    }
#endif
}