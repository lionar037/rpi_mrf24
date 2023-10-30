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

#define CMD_WRITE 0x2
#define CMD_READ 0x3

#define READ 0b011 //Read data from memory array beginning at selected address
#define WRITE 0b010 //Write data to memory array beginning at selected address
#define WRDI 0b100//Reset the write enable latch (disable write operations)
#define WREN 0b110//Set the write enable latch (enable write operations)
#define RDSR 0b101//Read STATUS
#define WRSR 0b001//Write STATUS register

namespace SPI{

  struct  Spi{

    Spi();
    ~Spi();

    void read_write();
    void init();
    void read_write(size_t,uint32_t);
    void settings_spi();
    void spi_close();
    uint8_t read(uint16_t);
    void write(uint16_t);
  
    void read_write_dbg();
    uint16_t transfer(uint16_t);

    uint8_t Transfer2bytes(uint16_t address);
    uint8_t Transfer3bytes(uint32_t address);

    void setBitOrder(const uint16_t);
    void setDataMode(const uint16_t);
    void begin();
    void printDBGSpi();
    void msj_fail();
    
  private:

    uint8_t tx_buffer[4];
    uint8_t rx_buffer[4];

    const uint32_t len_data = 32;
    const uint32_t spi_speed = 5000000;
    
    int fs{};
    int ret{};
    struct spi_ioc_transfer spi{};
    uint8_t looper{};
    uint32_t scratch32{};
  };

}//END namespace SPI_H
