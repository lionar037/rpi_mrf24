#pragma once
#include <app/work.h>

namespace OLED{

    struct Oled_t : public WORK::Work_t
    {
        Oled_t  ()=default;
        ~Oled_t ()=default;
        bool create(const char*)override;
    };

}