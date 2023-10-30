#pragma once
#include <iostream>

namespace TYME{

struct Time_t
{
    explicit Time_t();
    ~Time_t();
    void delay_ms(uint32_t);
    private:
        uint32_t m_delay{0};
};


}//end time