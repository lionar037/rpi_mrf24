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
#include <gpio.h>

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
    int Gpio::gpio_export(int gpio_num)
    {
        char gpio_str[4];
        sprintf(gpio_str, "%d", gpio_num);
        return file_open_and_write_value(SYSFS_GPIO_PATH SYSFS_GPIO_EXPORT_FN, gpio_str);
    }

    // GPIO UNEXPORT
    int Gpio::gpio_unexport(int gpio_num)
    {
        char gpio_str[4];
        sprintf(gpio_str, "%d", gpio_num);
        return file_open_and_write_value(SYSFS_GPIO_PATH SYSFS_GPIO_UNEXPORT_FN, gpio_str);
    }

    // GPIO DIRECTION
    int Gpio::gpio_set_direction(int gpio_num, const char *dir)
    {
        char path_str[40];
        sprintf(path_str, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_DIRECTION);
        return file_open_and_write_value(path_str, dir);
    }

    // GPIO SET VALUE
    int Gpio::gpio_set_value(int gpio_num, const char *value)
    {
        char path_str[40];
        sprintf(path_str, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_VALUE);
        return file_open_and_write_value(path_str, value);
    }

    // GPIO SET EDGE
    int Gpio::gpio_set_edge(int gpio_num, const char *edge)
    {
        char path_str[40];
        sprintf(path_str, "%s/gpio%d%s", SYSFS_GPIO_PATH, gpio_num, SYSFS_GPIO_EDGE);
        return file_open_and_write_value(path_str, edge);
    }

    int Gpio::gpio_get_fd_to_value(int gpio_num)
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

    const bool Gpio::app() {
        unsigned int gpio_out = OUT_INTERRUPT;
        struct pollfd fdpoll;
        int num_fdpoll = 1;
        int gpio_in = IN_INTERRUPT;
        int gpio_in_fd;
        int res;
        int looper = 0;
        char *buf[64];

    //std::cout << "Pin GPIO inp : "<< gpio_in<<"\n";
    //std::cout << "Pin GPIO out : "<< gpio_out<<"\n";

   const std::string filePathGpio23 = "/sys/class/gpio/gpio23/direction";
    std::ifstream fileGpio23(filePathGpio23);
    if(!fileGpio23){
        const bool result_input = std::system("echo 23 > /sys/class/gpio/export");
        if (result_input == 0) {
            std::cout << "Pin GPIO 23 exported successfully." << std::endl;
        } else {
            std::cerr << "Error exporting GPIO 23." << std::endl;
            return false;
        }
    }
   const std::string filePathGpio1 = "/sys/class/gpio/gpio23/direction";
    std::ifstream fileGpio1(filePathGpio1);
    if(!fileGpio1){
        const int result_output = std::system("echo 1 > /sys/class/gpio/export");
        if (result_output == 0) {
            std::cout << "Pin GPIO 1 exported successfully." << std::endl;
        } else {
            std::cerr << "Error unexporting GPIO 1." << std::endl;
            //return 0;//continua por que no es necesario el pin de salida
        }
    }

        gpio_unexport(gpio_out);
        gpio_unexport(gpio_in);

        gpio_export(gpio_out);
        gpio_export(gpio_in);

        gpio_set_direction(gpio_out,DIR_OUT);
        gpio_set_direction(gpio_in,DIR_IN);

        gpio_set_value(gpio_out,VALUE_HIGH);
        gpio_set_edge(gpio_in,EDGE_FALLING);

        gpio_in_fd = gpio_get_fd_to_value(gpio_in);

        // We will wait for button press here for 10s or exit anyway
        while(looper<READING_STEPS) {
            memset((void *)&fdpoll,0,sizeof(fdpoll));
            fdpoll.fd = gpio_in_fd;
            fdpoll.events = POLLPRI;

            res = poll(&fdpoll,num_fdpoll,POLL_TIMEOUT);

            if(res < 0) {
                printf("Poll failed...%d\r\n",res);            
            }
            if(res == 0) {
                printf("\nPoll success...timed out or received button press...\r\n");
               // std::cout<<"Esperando msj mrf24j40...\n";
            }
            if(fdpoll.revents & POLLPRI) {
                lseek(fdpoll.fd, 0, SEEK_SET);
                read(fdpoll.fd, buf, 64);
                //printf("Received a button press...%d\r\n",looper);
                 std::cout<<"Esperando msj mrf24j40...\n";
            }
            ++looper;
            fflush(stdout);
        }

        close(gpio_in_fd);
        gpio_set_value(gpio_out,VALUE_LOW);
        gpio_unexport(gpio_out);
        gpio_unexport(gpio_in);

        return true;
    }


    //Gpio::Gpio(){
    //    #ifdef DBG
    //        std::cout<<"~Gpio()\n";
    //    #endif
    //    //app();
    //}
    
    // Gpio::Gpio(bool& status){
        #ifdef DBG
            // std::cout<<"Gpio(bool)\n";
        #endif
    // }
    

    Gpio::~Gpio(){
        #ifdef DBG
            std::cout<<"~Gpio()\n";
        #endif       
    }
}
