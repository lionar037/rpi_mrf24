#pragma once 
#include <string>

namespace SECURITY{

    Security_t{
        Security_t();
        ~Security_t();
        std::string sha256(const std::string&) ;
        std::string getHiddenInput() ;
        int function() ;
    private:

    };


}