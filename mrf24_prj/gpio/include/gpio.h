//////////////////////////////////////////////////////////////////////////////
//     
//          filename            :   gpio.h
//          License             :   GNU 
//          Author              :   Lio
//          Change History      :
//          Processor           :   ARM
//          Hardware            :		
//          Complier            :   ARM
//          Company             :
//          Dependencies        :
//          Description         :
//          brief               :	
//
//////////////////////////////////////////////////////////////////////////////


#pragma once
#include <fstream>
#include <iostream>
#include <string_view>
#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>

//#define DBG_GPIO

namespace GPIO {

#if defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4)
    // Si es de 32 bits
    constexpr int IN_INTERRUPT = 23;
    constexpr int OUT_INTERRUPT = 12;
#else
    //constexpr int IN_INTERRUPT = 531;//restarle 512
    //constexpr int OUT_INTERRUPT = 532;//restarle 512
    constexpr int IN_INTERRUPT = 23;
    constexpr int OUT_INTERRUPT = 12;
#endif


constexpr int READING_STEPS = 2;

const std::string SYSFS_GPIO_PATH = "/sys/class/gpio";
const std::string SYSFS_GPIO_EXPORT_FN = "/export";
const std::string SYSFS_GPIO_UNEXPORT_FN = "/unexport";
const std::string SYSFS_GPIO_VALUE = "/value";
const std::string SYSFS_GPIO_DIRECTION = "/direction";
const std::string SYSFS_GPIO_EDGE = "/edge";
const std::string DIR_IN = "in";
const std::string DIR_OUT = "out";
const std::string VALUE_HIGH = "1";
const std::string VALUE_LOW = "0";
const std::string EDGE_RISING = "rising";
const std::string EDGE_FALLING = "falling";

constexpr int POLL_TIMEOUT = 10 * 1000;

#define DBG_GPIO_PRINT(x) std::cout << "Step :" << (x) << "\n"

    struct GpioConform_t {
        int                 ID;
        uint16_t            gpio;
        std::string         dir;
        std::string         edge;
        std::string         value;
        std::ifstream       path;
        bool status;

        GpioConform_t() = default;
        GpioConform_t(int id, uint16_t g, std::string d, std::string e, std::string v, bool s)
            : ID(id), gpio(g), dir(std::move(d)), edge(std::move(e)), value(std::move(v)), status(s) {}
        
        GpioConform_t(GpioConform_t&&) noexcept = default;
        GpioConform_t& operator=(GpioConform_t&&) noexcept = default;

        GpioConform_t(const GpioConform_t&) = delete;
        GpioConform_t& operator=(const GpioConform_t&) = delete;
    };

    class Gpio_t {
    public:
        explicit Gpio_t(bool& status);
        ~Gpio_t();

        int         file_open_and_write_value(std::string_view fname, std::string_view wdata);
        int         gpio_export(int gpio_num);
        int         gpio_unexport(int gpio_num);
        int         gpio_set_direction(int gpio_num, std::string_view dir);
        int         gpio_set_value(int gpio_num, std::string_view value);
        int         gpio_set_edge(int gpio_num, std::string_view edge);
        int         gpio_get_fd_to_value(int gpio_num);
        void        CloseGpios();
        const bool app(bool&) ;

        int         digitalWrite(uint16_t pin, std::string_view st);
        int         pinMode(uint16_t number_gpio, std::string_view read_direction);
        int         digitalRead(int gpio);
        int         settings(int gpio, std::string_view direction, std::ifstream& filenameTmp);
        int         getNextId();
        void        printGpios() const;
        void        addGpio(uint16_t gpio_pin, std::string dir, std::string edge, std::string value);
       
    private:
        bool        m_state;
        int         m_gpio_in_fd;
        const unsigned int m_gpio_out = OUT_INTERRUPT;
        const int   m_gpio_in = IN_INTERRUPT;
        std::ifstream filenameGpio;
        
        std::unordered_map<int, GpioConform_t*> gpioById;
        std::unordered_map<uint16_t, GpioConform_t*> gpioByPin;

        std::vector<std::unique_ptr<GpioConform_t>> m_gpio_cfg;

        void updateGpioMaps();
        static inline int static_file_open_and_write_value{0};
        int m_res{};
    };

}