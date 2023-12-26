#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <display/src/oled.h>
#ifdef USE_MRF24_RX
#include <SSD1306_OLED.hpp>
#endif


//GLOBAL namespace OLED
namespace OLED{
#ifdef USE_MRF24_RX

SSD1306 myOLED(myOLEDwidth , myOLEDheight) ; // instantiate  an object         
static int count { 0 };

bool Oled_t::create(const std::string_view& textOnOled){
        
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
                bcm2835_delay(100);
                //myOLED.OLEDclearBuffer();
                return;
        }

        void Oled_t::End()
        {
                myOLED.OLEDPowerDown(); //Switch off display
                bcm2835_close(); // Close the library
                //printf("OLED End\r\n");
        return;
        }



        void Oled_t::Start()
        {
                
                // Define a buffer to cover whole screen 
                static uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight/8)+1]; 
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
                        //myOLED.setCursor(36, 14);
                        //printf(" count : %d\n",count);
                        //myOLED.print(reinterpret_cast<int>(count));
                        myOLED.OLEDupdate();
                        count++;
                        //myOLED.setRotation(3);
                        // myOLED.OLEDContrast(0x00);                        
                        // myOLED.OLEDContrast(0x80);                        
                        // myOLED.OLEDContrast(0xFF);
                        // bcm2835_delay(1000);
                        // myOLED.OLEDContrast(0x81);

                        while(1);
            
            return;
    }


        void  Oled_t::Graphics(const int x,const int y,const bool* z,const uint8_t* w){
                uint8_t buff [(x+3)*(y+3)];
                int l{-1},Position{0};
                //int module =0;

                //std::cout << "\033[" << "15" << ";" << "0" << "H" <<"\n";  

                //for(int i=0 ; i< (y+3)*(x+3); i++){buff[l++]=0x00;}
                //l=-1;

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
        }



    Oled_t::Oled_t(){
        init();        
    }

    Oled_t::~Oled_t(){
        End();
    }
#endif
}