 void  Oled_t::Graphics(const int x,const int y,const bool* z,const uint8_t* w){
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
        }
