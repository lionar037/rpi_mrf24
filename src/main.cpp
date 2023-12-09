/*
author : lion
*/
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <mrf24/src/radio.h>


int main(){
    auto mrf { std::make_unique<MRF24J40::Radio_t>() };
    std::vector<std::thread> threadVect;
const std::size_t ThreadCant{3};


    try
    {

        for (std::size_t i = 0; i < ThreadCant; ++i) {
            threadVect.emplace_back([&mrf]() {
           mrf->Run();
            });
        }

        // Esperar a que todos los hilos terminen
        for (auto& threadRun : threadVect) {
            threadRun.join();
        }      
    }
    catch(...){
        std::cerr<<"\nerror :(\n";
    }
    return 0 ;
}
