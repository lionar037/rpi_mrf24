extern "C"{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <errno.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <poll.h>
}

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string_view>

#include <others/src/gpio.h>
#include <app/src/config.h>


namespace GPIO{
    /*      HELPER FUNCTIONS       */
    // FILE OPERATION
    int Gpio::file_open_and_write_value(const char *fname, const char *wdata)
    {
        int fd;

        fd = open(fname, O_WRONLY | O_NONBLOCK);
        if (fd < 0)
        {
            printf("Could not open file %s...%d\r\n", fname, fd);
        }
        write(fd, wdata, strlen(wdata));
        close(fd);

        return 0;
    }

    // GPIO EXPORT
    int Gpio::gpio_export(const int gpio_num)
    {
        char gpio_str[4];
        sprintf(gpio_str, "%d", gpio_num);
        return file_open_and_write_value(SYSFS_GPIO_PATH SYSFS_GPIO_EXPORT_FN, gpio_str);
    }

    // GPIO UNEXPORT
    int Gpio::gpio_unexport(const int gpio_num)
    {
        char gpio_str[4];
        sprintf(gpio_str, "%d", gpio_num);
        return file_open_and_write_value(SYSFS_GPIO_PATH SYSFS_GPIO_UNEXPORT_FN, gpio_str);
    }

    // GPIO DIRECTION
    int Gpio::gpio_set_direction(const int gpio_num, const char *dir)
    {
        char path_str[40];
        sprintf(path_str, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_DIRECTION);
        return file_open_and_write_value(path_str, dir);
    }

    // GPIO SET VALUE
    int Gpio::gpio_set_value(const int gpio_num, const char *value)
    {
        char path_str[40];
        sprintf(path_str, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_VALUE);
        return file_open_and_write_value(path_str, value);
    }

    // GPIO SET EDGE
    int Gpio::gpio_set_edge(const int gpio_num, const char *edge)
    {
        char path_str[40];
        sprintf(path_str, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_EDGE);
        return file_open_and_write_value(path_str, edge);
    }

    int Gpio::gpio_get_fd_to_value(const int gpio_num)
    {
        int fd;
        char fname[40];
        sprintf(fname, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_VALUE);
        fd = open(fname, O_RDONLY | O_NONBLOCK);
        if (fd < 0)
        {
            printf("Could not open file %s...%d\r\n", fname, fd);
        }
        return fd;
    }



    bool Gpio::settings(const int pin , const std::string_view str_v){
           const std::string filePathGpio = "/sys/class/gpio/gpio" + std::to_string(pin) + "/direction";
        std::ifstream fileGpio(filePathGpio);
        if(!fileGpio){
            const std::string f("echo " + std::to_string(pin) + " > /sys/class/gpio/export");
            const int result_output = std::system(f.c_str());
            if (result_output == 0) {
                #ifdef DBG_GPIO
                    std::cout << "Pin GPIO "+ std::to_string(pin) +" exported successfully." << std::endl;
                #endif
            } else {
                #ifdef DBG_GPIO
                    std::cerr << "Error unexporting GPIO "+ std::to_string(pin) +"." << std::endl;
                #endif
                return false;
            }
        }
        gpio_unexport(pin);        
        gpio_export(pin);        
        gpio_set_direction(pin,str_v.data());
        return true;
    }

    const bool Gpio::app(bool& flag) 
    {
        const unsigned int gpio_out = OUT_INTERRUPT;
        struct pollfd fdpoll;
        int num_fdpoll = 1;
        const int gpio_in = IN_INTERRUPT;
      //  int gpio_in_fd;
        int res;
        int looper = 0;
        char *buf[64];

    // std::cout << "Pin GPIO inp : "<< gpio_in<<"\n";
    // std::cout << "Pin GPIO out : "<< gpio_out<<"\n";
{
        //Led Debugger
        settings(gpio_out ,DIR_OUT );
        settings(gpio_in , DIR_IN);

        gpio_set_value(gpio_out,VALUE_HIGH);
        gpio_set_edge(gpio_in,EDGE_FALLING);

        const int gpio_in_fd = gpio_get_fd_to_value(gpio_in);

 
        // We will wait for button press here for 10s or exit anyway
        if(state==true)
        {

        while(looper<READING_STEPS) {
            memset((void *)&fdpoll,0,sizeof(fdpoll));
            fdpoll.fd = gpio_in_fd;
            fdpoll.events = POLLPRI;
            res = poll(&fdpoll,num_fdpoll,POLL_TIMEOUT);

            if(res < 0) {
                #ifdef DBG_GPIO
                printf("Poll failed...%d\r\n",res);   
                #endif         
                }
            if(res == 0) {
                #ifdef DBG_GPIO
                    std::cout<<"\nPoll success...timed out or received button press...\r\n";
                #endif
                }
            if(fdpoll.revents & POLLPRI) {
                lseek(fdpoll.fd, 0, SEEK_SET);
                read(fdpoll.fd, buf, 64);
                #ifdef DBG_GPIO
                    std::cout<<"Standby reading msj mrf24j40...\n";
                #endif
                }
            ++looper;
            fflush(stdout);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));   
        }
        else{
            gpio_set_value(gpio_out,VALUE_HIGH);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));            
        }    
        close(gpio_in_fd);
        gpio_set_value(gpio_out,VALUE_LOW);
        gpio_unexport(gpio_out);
        gpio_unexport(gpio_in);
    }
        return false;
    }



    Gpio::~Gpio(){
        #ifdef DBG
            std::cout<<"~Gpio()\n";
        #endif       
    }
}
