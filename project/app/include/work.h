#pragma once
#include <cstring>
#include <string_view>

namespace WORK{
    struct Work_t{
            //virtual bool    create(const char*)=0;  
            virtual bool create(const std::string_view&)=0;
            //virtual std::string create(const char*)=0;       
            virtual         ~Work_t()=default;
        private:
    };


    
}

