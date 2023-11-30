
#include <spi/spi.h>
#include <spi/config.h>

namespace SPI{

    void Spi::msj_fail(){
            #ifdef DBG
                printf("Could not open the Spi device...\r\n");
            #endif
            return;
        }

    void Spi::printDBGSpi(){    
        #ifdef DBG_BUFFER
            std::cout << " spi.tx_buf   hex : 0x"<< std::hex<< spi->tx_buf <<"\n";
            std::cout << " spi.rx_buf   hex : 0x"<< std::hex<< spi->rx_buf <<"\n";
        #endif
        #ifdef DBG
            std::cout << " spi.len  : "<< std::dec<<spi->len <<"\n";
            std::cout << " spi.delay_usecs  : "<< std::dec<<spi->delay_usecs <<"\n";
            std::cout << " spi.speed_hz  : "<< std::dec<<spi->speed_hz <<"\n";
            printf("spi.bits_per_word :%d \n", spi->bits_per_word);
            printf("spi.cs_change :%d \n", spi->cs_change);
            std::cout << " spi.bits_per_word  : "<< std::dec<< spi->bits_per_word <<"\n";
            std::cout << " spi.cs_change : "<< std::dec<< spi->cs_change <<"\n";
        #endif
        
            #ifdef DBG_SPI
                if (rx_buffer[2]!=0xff)printf("rx buffer dec : %d\r\n"  ,rx_buffer[2]);
                printf("\tTransfer3bytes - SPI transfer returned : %d ...\r\n", ret);
            #endif
    }

}