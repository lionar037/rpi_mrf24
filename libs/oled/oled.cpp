#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include "SSD1306_OLED.hpp"
#include <oled/oled.h>


namespace OLED{


SSD1306 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object 


        bool Oled_t::create(const char* tmp)
        {
            return true;
        }

        bool Oled_t::init(){
                    if(!bcm2835_init())
        {
                printf("Error 1201: init bcm2835 library\r\n");
                return -1;
        }

        SetupTest();
        //TestLoop();
        EndTest();

        return true;
        }


        // ===================== Function Space =====================
void Oled_t::SetupTest() 
{
        bcm2835_delay(100);
        printf("OLED Begin\r\n");
        myOLED.OLEDbegin(I2C_Speed, I2C_Address); // initialize the OLED
//      myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
        bcm2835_delay(100);
}

void Oled_t::EndTest()
{
        myOLED.OLEDPowerDown(); //Switch off display
        bcm2835_close(); // Close the library
        printf("OLED End\r\n");
}



 void Oled_t::TestLoop()
{

        // Define a buffer to cover whole screen 
        uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight/8)+1]; 
        myOLED.buffer = (uint8_t*) &screenBuffer;  // set that to library buffer pointer

        myOLED.OLEDclearBuffer();  
        myOLED.setTextColor(WHITE);
        myOLED.setCursor(0, 0);
        myOLED.print("MRF24J40");
        return;

}

}