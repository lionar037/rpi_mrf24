/*
author : amaya
*/


//#include <run.h>
#include<spi.h>
#include <memory>
int main(){
std::unique_ptr<SPI::Spi> spi { std::make_unique<SPI::Spi>() };
spi->Transfer3bytes(0x00E0C1);
return 0 ;
}
