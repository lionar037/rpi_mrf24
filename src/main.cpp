/*
author : lion
*/
#include <iostream>
#include <memory>
#include <thread>
#include <mrf24/src/radio.h>


int main(){
    auto mrf { std::make_unique<MRF24J40::Radio_t>() };

    try
    {
        mrf->Run();
    }
    catch(...){
        std::errc<<"\nerror :(\n";
    }
    return 0 ;
}
