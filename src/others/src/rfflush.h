#pragma once
#include <string_view>
#include <mutex>

namespace FFLUSH{

struct Fflush_t
{
        Fflush_t()=default;
        ~Fflush_t()=default;
        /* data */
        //friend void updateValue(int, int, int, int) ;
        //void updateValue(int, int, int, int) ;
        int funcThread() ;
        void print(std::string_view, int, int) ;
    //private:
    protected:
        std::mutex m_mtx;
};



}


