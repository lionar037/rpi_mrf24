/*
author : amaya
*/


#include <run.h>

int main(){

std::unique_ptr<MRF24J40::Run> run { std::make_unique<MRF24J40::Run>()};

return 0 ;
}
