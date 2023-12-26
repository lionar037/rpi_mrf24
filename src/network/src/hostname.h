#pragma once
#include <string>

namespace NETWORK
{


struct Hostname_t{
Hostname_t()=default;
~Hostname_t()=default;

void print();
void GetHostname(std::string&);

};



}