#include <iostream>
#include <run.h>
//#include <mrf24j40.h>

namespace MRF24J40{ 

Run::Run() {
    mrf24j40_spi.init();
    mrf24j40_spi.interrupt_handler();
    mrf24j40_spi.set_pan(0xcafe);
    mrf24j40_spi.address16_write(0x6001); 
    mrf24j40_spi.Transfer3bytes(0xE0C1);
    std::cout<<"Run()\n";
}

void Run::interrupt_routine() {
    mrf24j40_spi.interrupt_handler(); // mrf24 object interrupt routine
}


void handle_rx() {
    
    std::cout<<"received a packet ";
    std::cout<<std::dec << mrf24j40_spi.get_rxinfo()->frame_length;
    
    std::cout<<" bytes long";
    
    if(mrf24j40_spi.get_bufferPHY()){
      std::cout<<"Packet data (PHY Payload):";
      for (int i = 0; i < mrf24j40_spi.get_rxinfo()->frame_length; i++) {
          std::cout<<mrf24j40_spi.get_rxbuf()[i];
      }
    }
    
    std::cout<<"\r\nASCII data (relevant data):";
    for (int i = 0; i < mrf24j40_spi.rx_datalength(); i++) {

        std::cout<<mrf24j40_spi.get_rxinfo()->rx_data[i];
    }
    
    std::cout<<"\r\nLQI/RSSI=";
    std::cout<<std::dec<<mrf24j40_spi.get_rxinfo()->lqi;

    std::cout<<"/";
    std::cout<<std::dec<<mrf24j40_spi.get_rxinfo()->rssi;
    
}

    void handle_tx() {
        // if (mrf24j40_spi->get_txinfo()->tx_ok) {
             std::cout<<"TX went ok, got ack";
        // } else {
             std::cout<<"TX failed after ";
        //     std::cout<<mrf24j40_spi->get_txinfo()->retries;
             std::cout<<" retries\n";
        // }
    }


void Run::loop() {

    mrf24j40_spi.check_flags(&handle_rx, &handle_tx);

    unsigned long current_time = 100000;
    if (current_time - last_time > tx_interval) {
        last_time = current_time;
        std::cout<<"txxxing...";
        mrf24j40_spi.send16(0x4202, "abcd");
    }
}

    Run::~Run() {
        std::cout<<"~Run()\n";
    }
}



