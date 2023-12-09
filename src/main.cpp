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
    std::vector<std::thread> Thread;
const int ThreadCant{3};


    try
    {

        for (int i = 0; i < ThreadCant; ++i) {
            Thread.emplace_back([&mrf]() {
           mrf->Run();
            });
        }

        // Esperar a que todos los hilos terminen
        for (auto& hilo : Thread) {
            hilo.join();
        }



      
    }
    catch(...){
        std::cerr<<"\nerror :(\n";
    }
    return 0 ;
}
