#pragma once
#include <iostream>

namespace RUN{
    struct Run_t{
        Run_t() {std::cout<<"Runt_t() \n";}//=default;
        ~Run_t(){std::cout<<"~Runt_t() \n";}//=default;
        int start();
    };
}
