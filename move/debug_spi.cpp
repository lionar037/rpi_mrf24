
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
    spi.len = static_cast<size_t>(cmd)+1;;
    tx_buffer[0] = CMD_WRITE;
    tx_buffer[1]=cmd&0xff;
    tx_buffer[2]=(cmd>>8)&0xff;
    ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
    if(ret != 0)std::cout<<"error \n";
    return;
  }

  uint8_t Spi::read(uint16_t address){
    spi.len =  static_cast<size_t>(address)+1;
    tx_buffer[0]= CMD_READ;
    tx_buffer[1]=address&0xff;
    tx_buffer[2]=(address>>8)&0xff;

    ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
    if(ret != 0){return 0x00 ;}
    return rx_buffer[2];
    return;
  }

  void Spi::read_write(size_t size ,uint32_t cmd){

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
        return;
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


}