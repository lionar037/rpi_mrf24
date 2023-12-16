#pragma once
#include <string_view>
#include <app/src/work.h>

namespace EPAPER{
struct Epaper_t : public SPI::Spi , WORK::Work_t{
//struct Epaper_t : public WORK::Work_t{
    Epaper_t()=default;
    ~Epaper_t()=default;
    void init();    
    bool create(const std::string_view&)override;
};

}