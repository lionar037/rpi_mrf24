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
#include <memory>

#define SPI_SPEED   100000 

namespace SPI{
  struct  Spi{

    [[nodiscard]] explicit Spi();
      ~Spi();

    void init();
    void settings_spi();
    void spi_close();
    const uint8_t Transfer2bytes(const uint16_t address);
    const uint8_t Transfer3bytes(const uint32_t address);
    void printDBGSpi();
    void msj_fail();  

  private:
 const uint32_t m_spi_speed { 0 } ;
    uint8_t m_tx_buffer[4]{ 0 };
    uint8_t m_rx_buffer[4]{ 0 };

    const uint32_t m_len_data  { 32 };
//    const uint32_t m_spi_speed { 0 } ;

    int fs{0};
    int ret{0};

    uint8_t looper{0};
    uint32_t scratch32{0};
    std::unique_ptr<struct spi_ioc_transfer >spi{nullptr} ;
    //std::unique_ptr<struct spi_ioc_transfer >spi;
  };

}//END namespace SPI_H
