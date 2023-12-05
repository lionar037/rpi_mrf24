/*
author : lion
*/


#include <run.h>
#include <memory>
#include <mrf24/src/radio.h>


int main(){
    auto mrf { std::make_unique<MRF24J40::Radio_t>() };

    
    return 0 ;
}
