#pragma once 
#include <string>

namespace SECURITY{

   struct Security_t{
        Security_t()=default;
        ~Security_t()=default;
        std::string sha256(const std::string&) ;
        std::string getHiddenInput() ;
        int function() ;
    private:

    };


}