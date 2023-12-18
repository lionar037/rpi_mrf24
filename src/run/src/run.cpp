#include <iostream>
#include <thread>
#include <vector>

#include <run/src/radio.h>
#include <run/src/run.h>


namespace RUN{

void Run_t::start(){
        system("clear"); 
    auto mrf { std::make_unique<MRF24J40::Radio_t>()};
    // std::vector<std::thread> threadVect;
    

//std::cout << "\033[2J\033[H" << std::flush;
     try    {
// 
        // for (int i = 0; i < 4; ++i) {
            // threadVect.emplace_back([&mrf]() {
            mrf->Run();
            // });
        //}
// 
        //Esperar a que todos los hilos terminen
        // for (auto& threadRun : threadVect) {
            // threadRun.join();
        // }      
      }
       catch(...){
         std::cerr<<"\nerror :(\n";
      }
}



}