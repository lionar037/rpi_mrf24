#pragma once
#include <string>

namespace NETWORK{
    struct Hostname_t{
    public:        
        Hostname_t()=default;
        ~Hostname_t()=default;
        void print();
        void GetHostname(std::string&);
    };

}