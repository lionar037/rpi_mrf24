#pragma once
#include <iostream>
#include <cstring>
#include <string_view>
#include <chrono>

namespace TYME{

struct Time_t
{
    explicit Time_t();
    ~Time_t();
    void delay_ms(const uint32_t);
    const std::string get_tyme();
    bool getHourNTP(const std::string_view& , std::chrono::system_clock::time_point& ) ;
    int timeUpdate() ;
    private:
        uint32_t m_delay{0};
};


}//end time