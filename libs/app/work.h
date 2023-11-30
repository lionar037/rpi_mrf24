#pragma once
#include <cstring>

namespace WORK{

    struct Work_t{
            virtual bool    create(const char*)=0;  
            //virtual std::string    create(const char*);       
            virtual         ~Work_t()=default;
        private:
    };
}