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

#include <chrono>
#include <thread>
#include <string_view>
#include <string>
#include <others/src/gpio.h>
#include <app/src/config.h>


namespace GPIO{
    /*      HELPER FUNCTIONS       */
    // FILE OPERATION
    int Gpio::file_open_and_write_value(const std::string_view fname, const std::string_view wdata)
    {
        //@params : verifica que exista el pin . Si no existe ,retorna -1 .
        const int fd = open(fname.data(), O_WRONLY | O_NONBLOCK);
        printf("fname.data() : %s\n",fname.data());
         //DBG_GPIO_PRINT(9);
        if (fd < 0)
        {
            printf("Could not open file %s...%d\r\n", fname.data(), fd);
            return -1;
        }
         //DBG_GPIO_PRINT(10);
        write(fd, wdata.data(), strlen(wdata.data()));
        
        close(fd);

        return 0;
    }


    //  int Gpio::file_open_and_write_value(const std::string_view fname, const std::string_view wdata) {
        // std::ofstream file(fname.data(), std::ios::out | std::ios::binary);
        // if (!file.is_open()) {
            // std::cerr << "Error: Could not open file " << fname.data() << std::endl;
            // return -1;
        // }
// 
        // file.write(wdata.data(), static_cast<std::streamsize>(wdata.length()));
        // file.close();
// 
        // return 0;
    // }

    // GPIO EXPORT
    int Gpio::gpio_export(const int gpio_num)
    {
        char gpio_str[4];
        sprintf(gpio_str, "%d", gpio_num);
          //std::cout<<"gpio_export\n";
        return file_open_and_write_value(SYSFS_GPIO_PATH SYSFS_GPIO_EXPORT_FN, gpio_str);
    }

    // GPIO UNEXPORT
    int Gpio::gpio_unexport(const int gpio_num)
    {
        char gpio_str[4];
        sprintf(gpio_str, "%d", gpio_num);
        //std::cout<<"dbg gpio_unexport "<< gpio_num <<"\n";
        return file_open_and_write_value(SYSFS_GPIO_PATH SYSFS_GPIO_UNEXPORT_FN, gpio_str);
    }

    // GPIO DIRECTION
    int Gpio::gpio_set_direction(const int gpio_num, const std::string_view dir)
    {
        char path_str[40];
        sprintf(path_str, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_DIRECTION);
        //std::cout<<"gpio_set_direction\n";
        return file_open_and_write_value(path_str, dir.data());
    }

    // GPIO SET VALUE
    int Gpio::gpio_set_value(const int gpio_num, const std::string_view value)
    {
        char path_str[40];
        sprintf(path_str, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_VALUE);
        //std::cout<<"gpio_set_value\n";
        // DBG_GPIO_PRINT(8);
        return file_open_and_write_value(path_str, value.data());
    }

    // GPIO SET EDGE
    int Gpio::gpio_set_edge(const int gpio_num, const std::string_view edge)
    {
        char path_str[40];
        sprintf(path_str, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_EDGE);
        //std::cout<<"gpio_set_edge\n";
        return file_open_and_write_value(path_str, edge.data());
    }

    int Gpio::gpio_get_fd_to_value(const int gpio_num)
    {
        int fd;
        char fname[64];
        sprintf(fname, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_VALUE);
        fd = open(fname, O_RDONLY | O_NONBLOCK);
        //printf("name : %s\n",fname);
        if (fd < 0)
        {
            printf("Could not open file %s... %d\r\n", fname, fd);
        }
        return fd;
    }



    bool Gpio::settings(const int pin , const std::string_view str_v ,std::ifstream& fileGpioTmp){
        const std::string filePathGpio = "/sys/class/gpio/gpio" + std::to_string(pin) + "/direction";
        //std::ifstream fileGpio(filePathGpio);
        fileGpioTmp.open(filePathGpio.c_str());
           //fileGpio.open(filePathGpio.c_str());
        if(!fileGpioTmp){
            const std::string fNameResult("echo " + std::to_string(pin) + " > /sys/class/gpio/export");
            const int result_output = std::system(fNameResult.c_str());
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
        const int gpio_out = OUT_INTERRUPT;//originalmente es unsigned 
        const int gpio_in = IN_INTERRUPT;
        struct pollfd fdpoll;
        int num_fdpoll = 1;
        
      //  int gpio_in_fd;
        int res;
        int looper = 0;
        char *buf[64];

{
//    DBG_GPIO_PRINT(1);

    gpio_set_direction(gpio_out,DIR_OUT);
    gpio_set_direction(gpio_in,DIR_IN);

//DBG_GPIO_PRINT(2);
        gpio_set_value(gpio_out,VALUE_HIGH);
        gpio_set_edge(gpio_in,EDGE_FALLING);
          
//DBG_GPIO_PRINT(3);
        const int gpio_in_fd = gpio_get_fd_to_value(gpio_in);
        m_gpio_in_fd = gpio_in_fd;
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
            //std::cout<<"else\n";
            gpio_set_value(gpio_out,VALUE_HIGH);
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));            
        }    
        
        //DBG_GPIO_PRINT(4);
        Clear();
        //DBG_GPIO_PRINT(5);
        
    }
        return false;
    }

    void Gpio::Clear()
    {

        close(m_gpio_in_fd);
        //DBG_GPIO_PRINT(6);
        gpio_set_value(m_gpio_out,VALUE_LOW);
        //DBG_GPIO_PRINT(7);
        gpio_unexport(m_gpio_out);
        gpio_unexport(m_gpio_in);
    }

    Gpio::~Gpio(){
            
            Clear();            
            
        #ifdef DBG
            std::cout<<"~Gpio()\n";
        #endif       
    }
}
