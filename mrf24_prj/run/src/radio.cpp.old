
#include <run/include/radio.h>
///#include <mrf24/include/mrf24j40.h>
#include <qr/include/qr.h>
#include <files/include/file.h>
#include <others/include/color.h>
#include <app/include/debugger.h>

//#include <mrf24/include/mrf24j40_template.tpp>
#ifdef USE_OLED
    #include <display/include/oled.h>
#endif
#include <others/include/rfflush.h>
#include <network/include/mosquitto.h>
#include <string_view>

namespace MRF24J40{ 

    std::string msj_txt; //es static              
    uint64_t m_add; 

    struct DATA::packet_tx buffer_transmiter;

    std::unique_ptr< MOSQUITTO::Mosquitto_t > Radio_t::mosq = nullptr;
    std::unique_ptr < Mrf24j >mrf24j40_spi = nullptr ;

    #ifdef ENABLE_SECURITY
        std::unique_ptr< SECURITY::Security_t > Radio_t::security = nullptr;        
    #endif

    Radio_t::Radio_t() 
    #ifdef ENABLE_INTERRUPT_MRF24
    :   m_status          (true)
    ,   fs              { std::make_unique<FILESYSTEM::File_t>() }
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
            mrf24j40_spi  = std::make_unique<Mrf24j>() ;        
        #ifdef ENABLE_SECURITY   
            security    =   std::make_unique<SECURITY::Security_t >();
        #endif
            
        mrf24j40_spi->init();
        mrf24j40_spi->settingsSecurity();
        mrf24j40_spi->interrupt_handler();
        mrf24j40_spi->set_pan(PAN_ID);
        // This is _our_ address

        #ifdef MACADDR16
            mrf24j40_spi->address16_write(ADDRESS); 
        #elif defined (MACADDR64)
            mrf24j40_spi->address64_write(ADDRESS_LONG);
        #endif

        // uncomment if you want to receive any packet on this channel
        mrf24j40_spi->set_promiscuous(true);

        mrf24j40_spi->settings_mrf();//esta linea estaba comentada originalmente

        // uncomment if you want to enable PA/LNA external control
        mrf24j40_spi->set_palna(true);

        // uncomment if you want to buffer all PHY Payload
        mrf24j40_spi->set_bufferPHY(true);

        //attachInterrupt(0, interrupt_routine, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
        //last_time = millis();

        //Single send cmd
        //mrf24j40_spi->Transfer3bytes(0xE0C1);
        mosq  =  std::make_unique<MOSQUITTO::Mosquitto_t>();
        #ifdef ENABLE_SECURITY
            Radio_t::security  = std::make_unique< SECURITY::Security_t > ();
        #endif
        m_flag=true;
    }

    bool Radio_t::Run(void){
        //std::cout << "\033[2J\033[H" << std::flush;
        //system("clear");
        gpio->app(m_flag);      
        Start(m_flag);                
        interrupt_routine() ;    
        std::cout <<"flag : "<< std::to_string(m_flag) <<"\n";
        return m_flag; 
    }



void Radio_t::Start(bool& flag) {

    flag = mrf24j40_spi->check_flags(&handle_rx, &handle_tx);
    const unsigned long current_time = 1000000;//1000000 original
    if (current_time - m_last_time > m_tx_interval) {
        m_last_time = current_time;
    #ifdef MRF24_TRANSMITER_ENABLE   //if si es TX 
            #ifdef ENABLE_SECURITY 
             if( security->init() != SUCCESS_PASS){
                std::cout<<"security->init : Exit Tx\n";
                return ; 
                }
                else{ std::cout<<"security->init : Success Tx\n"; }
            #endif
        #ifdef DBG_RADIO
            #ifdef MACADDR64
                std::cout<<"send msj 64() ... \n";
            #else
                std::cout<<"send msj 16() ... \n";
            #endif
        #endif
        buffer_transmiter.head=HEAD; 
        buffer_transmiter.size=(~strlen(MSJ))&0xffff ;
        #ifdef DBG_RADIO
            std::cout<<"\n strlen(MSJ) : "<<  strlen(MSJ)<<"\n";  
        #endif  
        std::strcpy(buffer_transmiter.data , MSJ);

        const char* msj = reinterpret_cast<const char* >(&buffer_transmiter);
        //  const auto* buff {reinterpret_cast<const char *>(mrf24j40_spi->get_rxinfo()->rx_data)};
        #ifdef DBG_RADIO
        std::cout<<"\n MSJ : size ( "<<  strlen(msj) <<" , "<<sizeof(msj) << " )\n" ;
        std::cout<<"\n" ;
        #endif
        const std::string pf(msj);
        #ifdef DBG_RADIO
            for(const auto& byte : pf) std::cout << byte ; 
        #endif
        std::cout<<"\n" ;         
        #ifdef USE_MRF24_TX 
            #ifdef MACADDR64
                mrf24j40_spi->send(ADDRESS_LONG_SLAVE, msj);               
              #ifdef DBG_RADIO
                std::cout<<"mrf24j40_spi->send 64 \n";
              #endif                
            #elif defined(MACADDR16)
                mrf24j40_spi->send(ADDRESS_SLAVE, msj);                                
              #ifdef DBG_RADIO
                std::cout<<"mrf24j40_spi->send 16 \n";
              #endif                
            #endif
                      
         const auto status = mrf24j40_spi->read_short(0x48);//or TXNSTAT =0: Transmissionwassuccessful         
//       const auto status = mrf24j40_spi->getStatusInfoTx();//mrf24j40_spi->check_ack(&handle_tx);
          if (status==0) {//0 = no Succeeded
              std::cout<<"\nTX ACK failed\n";
              #ifdef DBG_RADIO
              #endif
              return;
          } 
          if (status==1)  {//1 = Succeeded
              std::cout<<"\tTX ACK Ok   \n";
            #ifdef DBG_RADIO
                std::cout<<" retries : "<<std::to_string(mrf24j40_spi->get_txinfo()->retries);
                std::cout<<"\n";
            #endif
        }
        #endif
    #endif    
    }
}

    void Radio_t::interrupt_routine() {
        mrf24j40_spi->interrupt_handler(); // mrf24 object interrupt routine
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
    std::cout<<"\thandle_tx() : \n";
    const auto status = mrf24j40_spi->read_short(0x48);

         if (status) {
             std::cout<<"\t\t TX went ok, got ACK success ! \n";
         } else {
            std::cout<<"\n\tTX failed after \n";
            std::cout<<"retries : "<<mrf24j40_spi->get_txinfo()->retries;
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
    sprintf(bufferMonitor,"0x%x\n",mrf24j40_spi->get_rxinfo()->frame_length);
    monitor->print(bufferMonitor,files++,col);//    std::cout << " bytes long " ;
    
    if(mrf24j40_spi->get_bufferPHY()){
        monitor->print(" Packet data (PHY Payload) :",files++,col);//  std::cout << " Packet data (PHY Payload) :";
      #ifdef DBG_PRINT_GET_INFO
      for (int i = 0; i < mrf24j40_spi->get_rxinfo()->frame_length; i++) 
      {        
          std::cout <<" "<<std::hex<< mrf24j40_spi->get_rxbuf()[i];//monitor->set(" Packet data (PHY Payload) :",files,col);
      }
      #endif
    }
        //std::cout << "\n";        
        monitor->print("ASCII data (relevant data) :",files++,col); //std::cout<<"\r\nASCII data (relevant data) :\n";
        const auto recevive_data_length = mrf24j40_spi->rx_datalength();
        monitor->print("\tdata_length : " + std::to_string(recevive_data_length) ,files,col+36);        
        monitor->print("\n",files++,col);        
        const auto fil0=reinterpret_cast<const char*>(mrf24j40_spi->get_rxinfo()->rx_data );
        monitor->print( fil0 , files++ , col );
        //for (auto& byte : mrf24j40_spi->get_rxinfo()->rx_data)std::cout<<byte;        
        monitor->print("\n",files++,col);
        
    #ifdef DBG_PRINT_GET_INFO 
    
        if(ADDRESS_LONG_SLAVE == m_add){
            monitor->print("\nmac es igual\n" ,files++,col);
        }
        else{
            monitor->print("\nmac no es igual\n",files++ ,col) ;
        }
        std::string txt ="\ndata_receiver->mac : " + std::to_string (m_add )+ "\n" ;

        monitor->print(txt.c_str() , files++ ,col);
        auto packet_data_tmp = buffer_transmiter;
        txt ="buffer_receiver->head : " + std::to_string(packet_data_tmp.head) + "\n";
        monitor->print(txt.c_str() , files++ ,col);        
        auto bs = (~packet_data_tmp.size)&0xffff;//ver cual es la correcta ~ o no         
        txt = "buffer_receiver.size : " + std::to_string(bs) + "\n" ;        
        monitor->print( txt.c_str() ,files++,col);
        txt = "\nbuff: \n";  
        monitor->print( txt.c_str() ,files++,col);
        monitor->print("\r\n" ,files++,col);
        
    #endif            
        monitor->print("LQI : " + std::to_string(mrf24j40_spi->get_rxinfo()->lqi) ,files++,col);
        monitor->print("RSSI : " + std::to_string(mrf24j40_spi->get_rxinfo()->rssi) ,files++,col);  //std::cout<<"\r\n";
    #endif                
        const int temperature = mosq->pub();
        const std::string temperatureToString=  "{ temp :" + std::to_string(temperature)+ " }";
        update(reinterpret_cast<const char*>(mrf24j40_spi->get_rxinfo()->rx_data) ); //update(tempString.data());                
        //std::cout<<temperatureToString.data(); 
        monitor->print(temperatureToString.data(),files++,col+36);
        
        msj_txt = reinterpret_cast<const char*>(mrf24j40_spi->get_rxinfo()->rx_data) ;
        
        monitor->print(msj_txt.c_str() , files++ , col+37 );
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



