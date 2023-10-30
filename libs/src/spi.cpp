
#include <spi.h>
#define SPI_DEVICE  "/dev/spidev0.0"

#define CMD_WRITE 0x2
#define CMD_READ 0x3

#define READ 0b011 //Read data from memory array beginning at selected address
#define WRITE 0b010 //Write data to memory array beginning at selected address
#define WRDI 0b100//Reset the write enable latch (disable write operations)
#define WREN 0b110//Set the write enable latch (enable write operations)
#define RDSR 0b101//Read STATUS
#define WRSR 0b001//Write STATUS register


namespace SPI {

void Spi::settings_spi(){
    spi.tx_buf = (unsigned long)tx_buffer;
    spi.rx_buf = (unsigned long)rx_buffer;

    spi.bits_per_word = 0;
    spi.speed_hz = spi_speed;
    spi.delay_usecs = 0;
    spi.len = 3;
  //  for(looper=0; looper<spi.len+1; ++looper) {
  //       tx_buffer[looper] = 0x00;
  //       rx_buffer[looper] = 0xFF;
  //   }
  return;
}


void Spi::init(){
fs = open(SPI_DEVICE, O_RDWR);
if(fs < 0) {
    msj_fail();
    exit(EXIT_FAILURE);
  }
  ret = ioctl(fs, SPI_IOC_RD_MODE, &scratch32);
if(ret != 0) {
      msj_fail();
      close(fs);
      exit(EXIT_FAILURE);
  }
  scratch32 |= SPI_MODE_0;
  ret = ioctl(fs, SPI_IOC_WR_MODE, &scratch32);   //SPI_IOC_WR_MODE32
if(ret != 0) {
    msj_fail();
    close(fs);
    exit(EXIT_FAILURE);
  }
	ret = ioctl(fs, SPI_IOC_RD_MAX_SPEED_HZ, &scratch32);
if(ret != 0) {
    close(fs);
    exit(EXIT_FAILURE);
  }
    scratch32 = spi_speed;
    ret = ioctl(fs, SPI_IOC_WR_MAX_SPEED_HZ, &scratch32);

    if(ret != 0) {
        msj_fail();
        close(fs);
        exit(EXIT_FAILURE);
    }
    return;
}




const uint8_t Spi::Transfer2bytes(const uint16_t cmd){
    spi.len = sizeof(cmd);
    rx_buffer[0]=rx_buffer[1]=0xff;
    rx_buffer[2]=rx_buffer[3]=0x00;
    memcpy(tx_buffer, &cmd, sizeof(cmd));
    ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
      if(ret != 0) { 
          printDBGSpi();
        return rx_buffer[2];
      }
  return 0x00;
  }


  const uint8_t Spi::Transfer3bytes(const uint32_t cmd){
    char buff_tmp[]={"0x00,0x00,0x00"};

    spi.len = sizeof(buff_tmp);
    rx_buffer[0]=rx_buffer[1]=rx_buffer[2]==0xff;
    rx_buffer[3]=0x00;
    memcpy(tx_buffer, &cmd, sizeof(cmd));
    ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
        if(ret != 0) {
            printDBGSpi();
          return rx_buffer[2];
          }
    return 0x00;
    }


    void Spi::spi_close(){
      std::cout<<"spi_close()\n";
        close(fs);
        std::cout<<"spi_close2()\n";
        exit(EXIT_SUCCESS);
        std::cout<<"spi_close3()\n";
        return;
    }

    Spi::Spi()
    {
      std::cout<<"Spi()\n";
        init();
        settings_spi();
        return;
    }




}//end namespace SPI_H
