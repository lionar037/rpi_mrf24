#pragma once
#include <fstream>

#define IN_INTERRUPT    23      //GPIO INTERRUPT 
#define OUT_INTERRUPT   12    //GPIO LED DBG
#define READING_STEPS   2     //10 originalmente

#define SYSFS_GPIO_PATH             "/sys/class/gpio"
#define SYSFS_GPIO_EXPORT_FN        "/export"
#define SYSFS_GPIO_UNEXPORT_FN      "/unexport"
#define SYSFS_GPIO_VALUE            "/value"
#define SYSFS_GPIO_DIRECTION        "/direction"
#define SYSFS_GPIO_EDGE             "/edge"

#define DIR_IN                      "in"
#define DIR_OUT                     "out"

#define VALUE_HIGH                  "1"
#define VALUE_LOW                   "0"

#define EDGE_RISING                 "rising"
#define EDGE_FALLING                "falling"

//#define POLL_TIMEOUT        10*1000
#define POLL_TIMEOUT        10*1000

#define DBG_GPIO_PRINT(x) std::cout<<"Step :"<<( x )<<"\n"

namespace GPIO{
struct Gpio{   
      Gpio(bool& st)
      : m_state   { st }
      {}

    ~Gpio();
            int file_open_and_write_value(const std::string_view, const std::string_view);
            int gpio_export(const int);
            int gpio_unexport(const int);
            int gpio_set_direction(const int , const std::string_view );
            int gpio_set_value(const int , const std::string_view);
            int gpio_set_edge(const int , const std::string_view);
            int gpio_get_fd_to_value(const int );
            const bool app(bool&) ;
            bool settings(const int , const std::string_view , std::ifstream& );
            void CloseGpios(void);
        private :
            static inline int static_file_open_and_write_value{0};
            bool m_state{false};
            int m_gpio_in_fd{};
            int m_res{};
            const unsigned int m_gpio_out{OUT_INTERRUPT};
            const int m_gpio_in{IN_INTERRUPT};
            std::ifstream filenameGpio;
            //std::ifstream fileGpioOutput;


    };
}