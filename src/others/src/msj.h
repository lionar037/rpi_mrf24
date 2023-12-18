#pragma once
#include <iostream>
#include <string>

namespace DEVICES{

    struct Msj_t{
         Msj_t(){}
        ~Msj_t(){}
        void get();
        void set(std::string&);
        void Start(){std::cout<<"Msj : Start() \n";}
    private:
        std::string txt;
    };



}