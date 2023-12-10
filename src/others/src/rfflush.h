#pragma once
#include <string_view>
#include <mutex>

namespace FFLUSH{

struct Fflush_t
{
        Fflush_t()=default;
        ~Fflush_t()=default;
        /* data */
        friend void updateValue(int, int, int, int) ;
        int func() ;
        void set(std::string_view, int, int) ;
    private:
        std::mutex mtx;
};



}


