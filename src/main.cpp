/*
author : amaya
*/


//#include <run.h>
#include<spi.h>
#include <memory>
int main(){
std::unique_ptr<SPI::Spi> _spi { std::make_unique<SPI::Spi>() };
//auto run { std::make_unique<MRF24J40::Run>()};

return 0 ;
}
