#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <vector>


namespace SPI{

  struct  Spi{

    Spi();
    ~Spi();

    //void read_write();
    void init();
    //void read_write(size_t,uint32_t);
    void settings_spi();
    void spi_close();
    //uint8_t read(uint16_t);
    //void write(uint16_t);
  
    //void read_write_dbg();
    //uint16_t transfer(uint16_t);

    uint8_t Transfer2bytes(uint16_t address);
    uint8_t Transfer3bytes(uint32_t address);

    //void setBitOrder(const uint16_t);
    //void setDataMode(const uint16_t);
    //void begin();
    void printDBGSpi();
    void msj_fail();
    
  private:

    uint8_t tx_buffer[4];
    uint8_t rx_buffer[4];

    const uint32_t len_data = 32;
    const uint32_t spi_speed = 5000000;
    
    int fs{0};
    int ret{0};
    struct spi_ioc_transfer spi{};
    uint8_t looper{0};
    uint32_t scratch32{0};
  };

}//END namespace SPI_H
