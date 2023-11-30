/*
author : amaya
*/


//#include <run.h>
#include <memory>
#include <mrf24/radio.h>

int main(){
    auto mrf { std::make_unique<MRF24J40::Radio_t>() };
    return 0 ;
}
