#pragma once 
#include <string>
#include <string_view>


#define PASSWORD_SAVE "0" 
#define SUCCESS_PASS    0
#define USER    "Admin"

namespace SECURITY{

   struct Security_t{
        Security_t()=default;
        ~Security_t()=default;
        std::string sha256(const std::string&) ;
        std::string getHiddenInput() ;
        int init() ;
        std::string encrypt(std::string_view&);
    private:
        std::string m_inputPassword;
        std::string m_user;

    };


}