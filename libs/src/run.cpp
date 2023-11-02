
#include <run.h>
#include <mrf24j40.h>

namespace MRF24J40{ 

Mrf24j mrf24j40_spi ;

Run::Run() 
{
    std::cout<<"Run()\n";
    mrf24j40_spi.init();
    mrf24j40_spi.interrupt_handler();
    mrf24j40_spi.set_pan(PAN_ID);
    // This is _our_ address
    mrf24j40_spi.address16_write(ADDRESS); 

    // uncomment if you want to receive any packet on this channel
  mrf24j40_spi.set_promiscuous(true);
  
    // uncomment if you want to enable PA/LNA external control
  mrf24j40_spi.set_palna(true);
  
    // uncomment if you want to buffer all PHY Payload
  mrf24j40_spi.set_bufferPHY(true);

    //attachInterrupt(0, interrupt_routine, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
    //last_time = millis();

    //mrf24j40_spi.Transfer3bytes(0xE0C1);

    loop();
}

void Run::loop() {
    mrf24j40_spi.check_flags(&handle_rx, &handle_tx);
    unsigned long current_time = 1000000;
    if (current_time - last_time > tx_interval) {
        last_time = current_time;
        //std::cout<<"txxxing...\n";
        std::cout<<"send16 () ... \n";
        mrf24j40_spi.send16(ADDR_SLAVE, "@aA");//send data//original//mrf24j40_spi.send16(0x4202, "abcd")
    }
}

void Run::interrupt_routine() {
    mrf24j40_spi.interrupt_handler(); // mrf24 object interrupt routine
}


void handle_rx() {
    
    std::cout << " \nreceived a packet ... ";
    printf("0x%x\n",mrf24j40_spi.get_rxinfo()->frame_length);
    std::cout << " bytes long " ;
    
    if(mrf24j40_spi.get_bufferPHY()){
      std::cout << " Packet data (PHY Payload) :";
      for (int i = 0; i < mrf24j40_spi.get_rxinfo()->frame_length; i++) {
          std::cout<<mrf24j40_spi.get_rxbuf()[i];
      }
    }
    
    std::cout<<"\r\nASCII data (relevant data) :\n";
    auto recevive_data_length = mrf24j40_spi.rx_datalength();
    std::cout << "\t\tdata_length : "<<std::dec<<recevive_data_length<<"\n\t";

    for (int i = 0; i < recevive_data_length; i++) {
        //std::cout<<std::hex<<mrf24j40_spi.get_rxinfo()->rx_data[i];
        printf("0x%X ",mrf24j40_spi.get_rxinfo()->rx_data[i]);
        //std::cout<<".";
    }
    //printf("\n%S\n ",mrf24j40_spi.get_rxinfo()->rx_data);


    std::cout<<"\r\nLQI/RSSI = \n\t";
    //std::cout<<std::dec<<mrf24j40_spi.get_rxinfo()->lqi;
    printf("0x%X ",mrf24j40_spi.get_rxinfo()->lqi);
    //std::cout<<" ";
    //std::cout<<std::hex<<mrf24j40_spi.get_rxinfo()->rssi;
    printf("0x%X ",mrf24j40_spi.get_rxinfo()->rssi);
}

void handle_tx() {
    auto status = mrf24j40_spi.get_txinfo()->tx_ok;
         if (status) {
             std::cout<<"TX went ok, got ack \n";
         } else {
             std::cout<<"\nTX failed after \n";
             std::cout<<mrf24j40_spi.get_txinfo()->retries;
             std::cout<<" retries\n";
         }
    }

    Run::~Run() {
        std::cout<<"~Run()\n";
    }
}



