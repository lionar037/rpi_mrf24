#pragma once
#include <iostream>
#include <cstring>

namespace TYME{

struct Time_t
{
    explicit Time_t();
    ~Time_t();
    void delay_ms(const uint32_t);
    const std::string get_tyme();
    private:
        uint32_t m_delay{0};
};


}//end time