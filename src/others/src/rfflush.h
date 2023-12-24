#pragma once
#include <string_view>
#include <mutex>
#include <vector>
//#include <ncurses.h>

namespace FFLUSH{

struct Fflush_t
{
    
        Fflush_t()=default;
            //initscr(); 
        
        ~Fflush_t()=default;
        /* data */
        //friend void updateValue(int, int, int, int) ;
        //void updateValue(int, int, int, int) ;
        int         funcThread      ( );
        void        print           ( std::string_view, int, int) ;
        void        sendMsj         ( std::string_view , int , int ) ;
        void        view            ( );
        
    private:
        static std::vector<std::string> memoria;
    protected:
        std::mutex m_mtx;

};



}


