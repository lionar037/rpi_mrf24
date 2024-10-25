#include <string_view>
#include <run/include/radio.h>
#include <mrf24/include/mrf24j40.h>
#include <qr/include/qr.h>
#include <files/include/file.h>
#include <others/include/color.h>
#include <mrf24/include/mrf24j40_template.tpp>
#ifdef USE_OLED
    #include <display/include/oled.h>
#endif
#include <others/include/rfflush.h>
#include <network/include/mosquitto.h>




namespace MRF24J40{ 

Mrf24j mrf24j40_spi ;

std::string msj_txt; //es static              


std::unique_ptr< MOSQUITTO::Mosquitto_t > Radio_t::mosq = nullptr;


#ifdef USE_MRF24_TX
    std::unique_ptr< SECURITY::Security_t > Radio_t::security = nullptr;
#endif

Radio_t::Radio_t() 
#ifdef ENABLE_INTERRUPT_MRF24
:   m_status          (true)
//,   fs              { std::make_unique<FILESYSTEM::File_t>() }
    #ifdef ENABLE_DATABASE
,   database        { std::make_unique<DATABASE::Database_t>() }
    #endif
#else
:   m_status          (false)
#ifdef ENABLE_QR
,   qr              { std::make_unique<QR::Qr_t>() }
#endif
#endif
,   gpio            { std::make_unique<GPIO::Gpio_t>(m_status) }

{
    
    #ifdef ENABLE_INTERRUPT_MRF24
    
    #else            
            security    =   std::make_unique<SECURITY::Security_t >();
    #endif
          
    #ifdef DBG
    std::cout << "Size msj : ( "<<std::dec<<sizeof(MSJ)<<" )\n";
    #endif

    mrf24j40_spi.init();
    mrf24j40_spi.settingsSecurity();
    mrf24j40_spi.interrupt_handler();
    mrf24j40_spi.set_pan(PAN_ID);
    // This is _our_ address

    #ifdef MACADDR16
        mrf24j40_spi.address16_write(ADDRESS); 
    #elif defined (MACADDR64)
        mrf24j40_spi.address64_write(ADDRESS_LONG);
    #endif

    // uncomment if you want to receive any packet on this channel
    mrf24j40_spi.set_promiscuous(true);
    //mrf24j40_spi.settings_mrf();
  
    // uncomment if you want to enable PA/LNA external control
    mrf24j40_spi.set_palna(true);
  
    // uncomment if you want to buffer all PHY Payload
    mrf24j40_spi.set_bufferPHY(true);

    //attachInterrupt(0, interrupt_routine, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
    //last_time = millis();

    //Single send cmd
    //mrf24j40_spi.Transfer3bytes(0xE0C1);
    
    mosq  =  std::make_unique<MOSQUITTO::Mosquitto_t>();
    
    m_flag=true;
}

    bool Radio_t::Run(void){
        //std::cout << "\033[2J\033[H" << std::flush;
        //system("clear");
                
        gpio->app(m_flag);                                  
        Start(m_flag);                
        interrupt_routine() ;  
        return m_flag; 
    }



void Radio_t::Start(bool& flag) {

    flag = mrf24j40_spi.check_flags(&handle_rx, &handle_tx);
    const unsigned long current_time = 1000000;//1000000 original
    if (current_time - m_last_time > m_tx_interval) {
        m_last_time = current_time;
    #ifdef MRF24_TRANSMITER_ENABLE   
            #ifdef ENABLE_SECURITY 
             if( security->init() != SUCCESS_PASS){
                std::cout<<"Exit tx\n";
                return ; 
                }
                else{ std::cout<<"Success tx\n"; }
            #endif
        #ifdef DBG
            #ifdef MACADDR64
                std::cout<<"send msj 64() ... \n";
            #else
                std::cout<<"send msj 16() ... \n";
            #endif
        #endif
        buffer_transmiter.head=HEAD; 
        buffer_transmiter.size=(~strlen(MSJ))&0xffff ;
        #ifdef ENABLE_PRINTS_DBG
        //std::cout<<"\n strlen(MSJ) : "<<  strlen(MSJ)<<"\n";  
        #endif  
        std::strcpy(buffer_transmiter.data , MSJ);

        const char* msj = reinterpret_cast<const char* >(&buffer_transmiter);
        //  const auto* buff {reinterpret_cast<const char *>(mrf24j40_spi.get_rxinfo()->rx_data)};
        #ifdef ENABLE_PRINTS_DBG
        //std::cout<<"\n MSJ : size ( "<<  strlen(msj) <<" , "<<sizeof(msj) << " )\n" ;
        //std::cout<<"\n" ;
      #endif
        const std::string pf(msj);
        #ifdef ENABLE_PRINTS_DBG
            for(const auto& byte : pf) std::cout << byte ; 
        #endif
        std::cout<<"\n" ;         
        #ifdef USE_MRF24_TX 
            #ifdef MACADDR64
                mrf24j40_spi.send(ADDRESS_LONG_SLAVE, msj);               
            #elif defined(MACADDR16)
                mrf24j40_spi.send(ADDRESS_SLAVE, msj);                                
            #endif
                      
         const auto status = mrf24j40_spi.read_short(0x48);//or TXNSTAT =0: Transmissionwassuccessful         
//         const auto status = mrf24j40_spi.getStatusInfoTx();//mrf24j40_spi.check_ack(&handle_tx);
          if (status==0) {
              std::cout<<"\nTX ACK failed\n";
          } 
          if (status==1)  {//0 = Succeeded
              std::cout<<"\tTX ACK Ok   \n";
            //  std::cout<<" retries : "<<std::to_string(mrf24j40_spi.get_txinfo()->retries);
            //  std::cout<<"\n";
        }
        #endif
    #endif
    
    }
}

    void Radio_t::interrupt_routine() {
        mrf24j40_spi.interrupt_handler(); // mrf24 object interrupt routine
    }

    void update(std::string_view str_view)
    {    
        const int positionAdvance{15};
        auto            fs          { std::make_unique<FILESYSTEM::File_t> () };
        #ifdef ENABLE_QR
        auto            qr_img      { std::make_unique<QR::Qr_img_t>() };
        #endif
        auto            monitor     { std::make_unique <FFLUSH::Fflush_t>()};

        const auto*     packet_data = reinterpret_cast<const char*>(str_view.data());

        std::string  PacketDataTmp (packet_data += positionAdvance);
        PacketDataTmp.resize(38);

        SET_COLOR(SET_COLOR_GRAY_TEXT);

        fs->create(packet_data);
        std::cout<<"\n";
        #ifdef ENABLE_QR
            qr_img->create(packet_data);            
        #endif
    return ;    
    }


void Radio_t::handle_tx() {    
    //const auto status = mrf24j40_spi.get_txinfo()->tx_ok;
    const auto status = mrf24j40_spi.read_short(0x48);

         if (status) {
             std::cout<<"\thandle_tx() : TX went ok, got ACK success ! \n";
         } else {
            std::cout<<"\n\tTX failed after \n";
            std::cout<<"retries : "<<mrf24j40_spi.get_txinfo()->retries;
            std::cout<<" \n";
         }
    return;
   
		// if(RadioStatus.TX_PENDING_ACK)									// if we were waiting for an ACK
		// {
			// uint8_t TXSTAT = lowRead(READ_TXSR); //#define READ_TXSR 0x48							// read TXSTAT, transmit status register
			// RadioStatus.TX_FAIL    = TXSTAT & 1;						// read TXNSTAT (TX failure status)
			// RadioStatus.TX_RETRIES = TXSTAT >> 6;						// read TXNRETRY, number of retries of last sent packet (0..3)
			// RadioStatus.TX_CCAFAIL = TXSTAT & 0b00100000;				// read CCAFAIL
			// RadioStatus.TX_PENDING_ACK = 0;								// TX finished, clear that I am pending an ACK, already got it (if I was gonna get it)
		// }
    }

 

 
void Radio_t::handle_rx() {        
    #ifdef MRF24_RECEIVER_ENABLE
    int files {POSITIOM_INIT_PRINTS};
    int col {0};
    char bufferMonitor[128];

    auto  monitor{std::make_unique <FFLUSH::Fflush_t>()};

    files=POSITIOM_INIT_PRINTS;

    monitor->print("received a packet ... ",files++,col);    //std::cout << " \nreceived a packet ... ";
    sprintf(bufferMonitor,"0x%x\n",mrf24j40_spi.get_rxinfo()->frame_length);
    monitor->print(bufferMonitor,files++,col);//    std::cout << " bytes long " ;
    
    if(mrf24j40_spi.get_bufferPHY()){
        monitor->print(" Packet data (PHY Payload) :",files++,col);//  std::cout << " Packet data (PHY Payload) :";
      #ifdef DBG_PRINT_GET_INFO
      for (int i = 0; i < mrf24j40_spi.get_rxinfo()->frame_length; i++) 
      {        
          //std::cout <<" "<<std::hex<< mrf24j40_spi.get_rxbuf()[i];//monitor->set(" Packet data (PHY Payload) :",files,col);
      }
      #endif
    }
        //std::cout << "\n";
        
        monitor->print("ASCII data (relevant data) :",files++,col); //std::cout<<"\r\nASCII data (relevant data) :\n";
        const auto recevive_data_length = mrf24j40_spi.rx_datalength();
        monitor->print("\tdata_length : " + std::to_string(recevive_data_length) ,files,col+36);        
        monitor->print("\n",files++,col);
        
        monitor->print(reinterpret_cast<const char*>(mrf24j40_spi.get_rxinfo()->rx_data ),files++,col);
        //for (auto& byte : mrf24j40_spi.get_rxinfo()->rx_data)std::cout<<byte;
        
        monitor->print("\n",files++,col);
        
        

    #ifdef DBG_PRINT_GET_INFO 
      //add = ADDRESS_LONG;
        if(ADDRESS_LONG_SLAVE == m_add){
            monitor->print("\nmac es igual\n" ,files++,col);
        }
        else{
            monitor->print("\nmac no es igual\n",files++ ,col) ;
        }
        monitor->print("\ndata_receiver->mac : " + std::to_string (m_add )+ "\n",files++ ,col);

        monitor->print("buffer_receiver->head : " + packet_data_tmp->head + "n",files++ ,col);
        auto packet_data_tmp = buffer_receiver;
        auto bs = (~packet_data_tmp->size)&0xffff;//ver cual es la correcta ~ o no 
        //auto bs = (packet_data_tmp->size)&0xffff;
        monitor->print("buffer_receiver->size : " + reinterpret_cast<const int *>(bs) + "\n" ,files++,col);
        monitor->print("data_receiver->data : " + reinterpret_cast<const char *>(packet_data_tmp->data) + "\n" ,files++,col);
        monitor->print("\nbuff: \n" + buff ,files++,col);
        monitor->print("\r\n" ,files++,col);
    #endif            
        monitor->print("LQI : " + std::to_string(mrf24j40_spi.get_rxinfo()->lqi) ,files++,col);
        monitor->print("RSSI : " + std::to_string(mrf24j40_spi.get_rxinfo()->rssi) ,files++,col);  //std::cout<<"\r\n";
    #endif
        
        
        const int temperature = mosq->pub();

        const std::string temperatureToString=  "{ temp :" + std::to_string(temperature)+ " }";

        update(reinterpret_cast<const char*>(mrf24j40_spi.get_rxinfo()->rx_data) ); //update(tempString.data());
        
        
        //std::cout<<temperatureToString.data(); 
        monitor->print(temperatureToString.data(),files++,col+36);
        
        msj_txt=reinterpret_cast<const char*>(mrf24j40_spi.get_rxinfo()->rx_data) ;
        
        monitor->maxLines(files);
        monitor->view();
                
        return;    
    }

    Radio_t::~Radio_t() {
        #ifdef DBG
            std::cout<<"~Radio_t()\n";
        #endif
    }
}



