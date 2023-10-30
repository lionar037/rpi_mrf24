#include <iostream>
#include <vector>
#include <spi.h>
#define SPI_DEVICE  "/dev/spidev0.0"

namespace SPI {

void Spi::settings_spi(){
    spi.tx_buf = (unsigned long)tx_buffer;
    spi.rx_buf = (unsigned long)rx_buffer;

    spi.bits_per_word = 0;
    spi.speed_hz = spi_speed;
    spi.delay_usecs = 0;
    spi.len = 3;
   for(looper=0; looper<spi.len+1; ++looper) {
        tx_buffer[looper] = 0x00;
        rx_buffer[looper] = 0xFF;
    }
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
}




uint8_t Spi::Transfer2bytes(uint16_t cmd){
    spi.len = sizeof(cmd);
    rx_buffer[0]=rx_buffer[1]=0xff;
    rx_buffer[2]=rx_buffer[3]=0xff;
    memcpy(tx_buffer, &cmd, sizeof(cmd));
    ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
      if(ret != 0) { 
          printDBGSpi();
        return rx_buffer[2];
      }
  return 0x00;
  }


  uint8_t Spi::Transfer3bytes(uint32_t cmd){
    char buff_tmp[3];
    spi.len = sizeof(buff_tmp);
    rx_buffer[0]=rx_buffer[1]=rx_buffer[2]==0xff;
    rx_buffer[3]=0xff;
    memcpy(tx_buffer, &cmd, sizeof(cmd));
    ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
        if(ret != 0) {
            printDBGSpi();
          return rx_buffer[2];
          }
    return 0x00;
    }

}//end namespace SPI_H
