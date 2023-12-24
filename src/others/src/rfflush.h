#pragma once
#include <string_view>
#include <mutex>
#include <vector>

namespace FFLUSH{

struct Fflush_t
{    
        Fflush_t()=default;                    
        ~Fflush_t()=default;
        /* data */        
                
        void        print           ( std::string_view, int, int) ;
        void        sendMsj         ( std::string_view , int , int ) ;
        void        view            ( );
        void        maxLines        ( int );

    private:
        static std::vector<std::string> memoria;
    protected:
        std::mutex m_mtx;

};



}


