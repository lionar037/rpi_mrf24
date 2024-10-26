/*
    author : lion
*/
#include <run/include/run.h>
#include <iostream>
#include <memory>

int main(){
    auto run{std::make_unique<RUN::Run_t>()};
    run->start();
    return 0 ;
}
