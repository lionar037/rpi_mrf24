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

#define POLL_TIMEOUT        10*1000


namespace GPIO{
struct Gpio{   
      Gpio(bool& st)
      : state   { st }
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
            void Clear(void);
        private :
            static inline int static_file_open_and_write_value{0};
            bool state{false};
            int m_gpio_in_fd{};
            const int m_gpio_out{OUT_INTERRUPT};
            const int m_gpio_in{IN_INTERRUPT};
            std::ifstream fileGpio;
            //std::ifstream fileGpio2;
    };
}