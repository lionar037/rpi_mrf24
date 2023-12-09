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
    std::vector<std::thread> hilos;



    try
    {

        for (int i = 0; i < numHilos; ++i) {
            hilos.emplace_back([&mrf]() {
           mrf->Run();
            });
        }

        // Esperar a que todos los hilos terminen
        for (auto& hilo : hilos) {
            hilo.join();
        }



      
    }
    catch(...){
        std::cerr<<"\nerror :(\n";
    }
    return 0 ;
}
