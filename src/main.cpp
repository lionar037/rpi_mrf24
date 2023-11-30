/*
author : amaya
*/


#include <run.h>
#include <memory>

int main(){
    std::unique_ptr<MRF24J40::Run> mrf { std::make_unique<MRF24J40::Run>() };

    return 0 ;
}
