#include <iostream>
#include <memory>
#include <unistd.h>
#include <others/src/rfflush.h>
#include <others/src/color.h>
#include <app/src/config.h>
#include <others/src/msj.h>

namespace FFLUSH{

static DEVICES::Msj_t msj;

    void Fflush_t::print(std::string_view str_txt, int row, int col) 
    {         
        std::string dat = "\033[" + std::to_string(row) + ";" + std::to_string(col) + "H" + str_txt.data();  
        msj.set(dat.data());
        //std::cout << "\033[" << row << ";" << col << "H"<<str_txt.data();            
        return ;
    }
    void Fflush_t::maxLines(int x){
        msj.setMaxLines(x);
    }

    void Fflush_t::view(){
        
        //#ifdef ENABLE_PRINTS_DBG  
        msj.printData();
        //#endif
    }
    
}


