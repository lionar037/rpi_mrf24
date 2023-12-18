#pragma once
#include <string>

namespace MSJ{

    struct Msj_t{
         Msj_t()=default;
        ~Msj_t()=default;
        void get();
        void set(std::string&);
    private:
        std::string txt;
    };



}