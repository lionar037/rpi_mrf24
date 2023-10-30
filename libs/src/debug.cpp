#include <iostream>
#include <spi.h>

//#define CS 0
//#define DBG_SPI
//#define PRINT_BUFFER
//#define SPI_DEVICE  "/dev/spidev0.0"

namespace SPI
{
  uint16_t Spi::transfer(uint16_t tx){
    spi.len = 2;
    tx_buffer[0]= tx & 0xFF;
    tx_buffer[1]=(tx>>8)&0xff;
    ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
    auto tmp =(rx_buffer[1] << 8) | rx_buffer[0];

    return tmp;
  }

  void Spi::write(uint16_t cmd){
    spi.len = 3;
    tx_buffer[0]= CMD_WRITE;
    tx_buffer[1]=cmd&0xff;
    tx_buffer[2]=(cmd>>8)&0xff;
    ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
    if(ret != 0)std::cout<<"error \n";
    return;
  }

  uint8_t Spi::read(uint16_t address){
    spi.len = 3;
    tx_buffer[0]= CMD_READ;
    tx_buffer[1]=address&0xff;
    tx_buffer[2]=(address>>8)&0xff;

    ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
    if(ret != 0){return 0x00 ;}
    return rx_buffer[2];
  }

  void Spi::read_write(size_t size ,uint32_t cmd){
    #ifdef DBG
      std::cout<<"size_t : "<< size <<"\n";
    #endif
      spi.len = size;
      tx_buffer[0]= cmd & 0xFF;
      tx_buffer[1]=(cmd>>8)&0xff;
      tx_buffer[2]=(cmd>>16)&0xff;
      ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
        #ifdef DBG
          printf("fs : status : %d\n" , fs);
        #endif
            if(ret != 0) {
        #ifdef DBG
          printf("SPI transfer returned %d...\r\n", ret);
        #endif
        }
        #ifdef PRINT_BUFFER
          #ifdef DBG
              printf("Received Spi buffer...\r\n");
          #endif
          for(looper=0; looper<4; ++looper)if(looper==2)printf(" rx_buffer : 0x%02x",rx_buffer[looper]);
        #endif
    printf("\n");
  }



    void Spi::msj_fail(){
        #ifdef DBG
            printf("Could not open the Spi device...\r\n");
            #endif
        return;
    }

    void Spi::read_write_dbg(){
        for(looper=0; looper<4; ++looper) {
            tx_buffer[looper] = 0xff;
            rx_buffer[looper] = 0xff;
        }
        #ifdef DBG
        printDBGSpi();
        #endif
        ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
        if(ret != 0) { }
        close(fs);
        exit(EXIT_SUCCESS);
    }

    void Spi::setBitOrder(const uint16_t _MSBFIRST) {
    return ;
    }

    void Spi::setDataMode(const uint16_t _SPI_MODE0){
    return ;
    }

    void Spi::begin(){
    return ;
    }


void Spi::printDBGSpi(){
     #ifdef DBG
        std::cout << " spi.tx_buf  : "<< spi.tx_buf<<"\n";
        std::cout << " spi.rx_buf  : "<< spi.rx_buf<<"\n";
        std::cout << " spi.len  : "<< spi.len<<"\n";
        std::cout << " spi.delay_usecs  : "<< spi.delay_usecs<<"\n";
        std::cout << " spi.speed_hz  : "<< spi.speed_hz<<"\n";
        printf("spi.bits_per_word :%d \n",spi.bits_per_word);
        printf("spi.cs_change :%d \n",spi.cs_change);
        std::cout << " spi.bits_per_word  : "<< std::hex<<spi.bits_per_word<<"\n";
        std::cout << " spi.cs_change : "<< std::hex<<spi.cs_change<<"\n";
    #endif
         #ifdef DBG_SPI
            if (rx_buffer[2]!=0xff)printf("rx buffer dec : %d\r\n"  ,rx_buffer[2]);
            printf("\tTransfer3bytes - SPI transfer returned : %d ...\r\n", ret);
          #endif
}




    void Spi::spi_close(){
        close(fs);
        exit(EXIT_SUCCESS);
    }

    Spi::Spi(){
        init();
        settings_spi();
    }

    Spi::~Spi(){
    spi_close();
    if(fs)close(fs);
    }


}