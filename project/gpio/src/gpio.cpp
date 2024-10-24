//////////////////////////////////////////////////////////////////////////////
//     
//          filename            :   gpio.cpp
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

#include <gpio/include/gpio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <unistd.h> 
#include <cstring>

extern "C"{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <errno.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <poll.h>
}


namespace GPIO {

    Gpio_t::Gpio_t(bool& status)
        : m_state(status), m_gpio_in_fd(-1) {
        #ifdef DBG_GPIO
        std::cout << "Gpio_t::Gpio_t()\n";
        #endif
    }

    Gpio_t::~Gpio_t() {
        CloseGpios();
        #ifdef DBG_GPIO
        std::cout << "~Gpio()\n";
        #endif
    }

    int Gpio_t::file_open_and_write_value(std::string_view fname, std::string_view wdata) {
        std::ofstream file(fname.data(), std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            #ifdef DBG_GPIO
            std::cerr << "El archivo no existe o no se pudo abrir : " << fname << std::endl;
            #endif
            return -1;
        }
        file << wdata;
        return 0;
    }

    int Gpio_t::gpio_export(int gpio_num) {

        #ifdef DBG_GPIO
        std::cout << "gpio_export()\n";
        #endif
        //std::string str = SYSFS_GPIO_PATH.data()  SYSFS_GPIO_EXPORT_FN.data() ;
         std::string path = std::string(SYSFS_GPIO_PATH) + std::string(SYSFS_GPIO_EXPORT_FN); 
        return file_open_and_write_value( path  , std::to_string(gpio_num));
    }

    int Gpio_t::gpio_unexport(int gpio_num) {
        #ifdef DBG_GPIO
        std::cout << "gpio_unexport()\n";
        #endif
        std::string path = std::string(SYSFS_GPIO_PATH) + std::string(SYSFS_GPIO_EXPORT_FN); 
        return file_open_and_write_value( path  , std::to_string(gpio_num));
    }

    int Gpio_t::gpio_set_direction(int gpio_num, std::string_view dir) {
        #ifdef DBG_GPIO
        std::cout << "gpio_set_direction() : " << dir << std::endl;
        #endif
        std::string path = std::string(SYSFS_GPIO_PATH) +"/gpio" +  std::to_string(gpio_num) + std::string(SYSFS_GPIO_DIRECTION); 
        return file_open_and_write_value( path, dir);
    }

    int Gpio_t::gpio_set_value(int gpio_num, std::string_view value) {
        #ifdef DBG_GPIO
        std::cout << "gpio_set_value()\n";
        #endif
        std::string path = std::string (SYSFS_GPIO_PATH) +  "/gpio" + std::to_string(gpio_num) + std::string (SYSFS_GPIO_VALUE);
        return file_open_and_write_value( path , value);
    }

    int Gpio_t::gpio_set_edge(int gpio_num, std::string_view edge) {
        #ifdef DBG_GPIO
        std::cout << "gpio_set_edge()\n";
        #endif
        std::string path = std::string (SYSFS_GPIO_PATH)  + "/gpio" + std::to_string(gpio_num) + std::string (SYSFS_GPIO_EDGE);
        return file_open_and_write_value( path , edge);
    }

    int Gpio_t::gpio_get_fd_to_value(int gpio_num) {
        #ifdef DBG_GPIO
        std::cout << "gpio_get_fd_to_value()\n";
        #endif
        const std::string fname = "/sys/class/gpio/gpio" + std::to_string(gpio_num) + "/value";
        if (!std::filesystem::exists(fname)) {
            #ifdef DBG_GPIO
            std::cerr << "El archivo " << fname << " no existe." << std::endl;
            #endif
            return -1;
        }
        std::ifstream file(fname);
        if (!file.is_open()) {
            #ifdef DBG_GPIO
            std::cerr << "No se pudo abrir el archivo " << fname << " para leer." << std::endl;
            #endif
            return -1;
        }
        std::string contenido((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        #ifdef DBG_GPIO
        std::cout << "Contenido actual del archivo " << fname << ":\n" << contenido << std::endl;
        #endif
        return std::stoi(contenido);
    }

    void Gpio_t::CloseGpios() {
        if (filenameGpio.is_open()) filenameGpio.close();
        if (m_gpio_in_fd != -1) {close(m_gpio_in_fd);}
        gpio_set_value(m_gpio_out, VALUE_LOW);
        gpio_unexport(m_gpio_out);
        gpio_unexport(m_gpio_in);
        #ifdef DBG_GPIO
        std::cout << "Gpio_t::CloseGpios()" << std::endl;
        #endif
    }

    int Gpio_t::digitalWrite(uint16_t pin, std::string_view st) {
        #ifdef DBG_GPIO
        std::cout << "digitalWrite : " << pin << " type : " << st << std::endl;
        #endif
        return gpio_set_value(pin, st);
    }

    int Gpio_t::pinMode(uint16_t number_gpio, std::string_view read_direction) {
        #ifdef DBG_GPIO
        std::cout << "pinMode : " << number_gpio << " Direction : " << read_direction << std::endl;
        #endif
        return gpio_set_direction(number_gpio, read_direction);
    }

    int Gpio_t::digitalRead(int gpio) {
        #ifdef DBG_GPIO
        std::cout << "digitalRead: " << gpio << std::endl;
        #endif
        return gpio_get_fd_to_value(gpio);
    }

    int Gpio_t::settings(int gpio, std::string_view direction, std::ifstream& filenameTmp) {
        filenameTmp.open("/sys/class/gpio/gpio" + std::to_string(gpio) + "/direction");
        if (!filenameTmp.is_open()) {
            #ifdef DBG_GPIO
            std::cerr << "No se pudo abrir el archivo de dirección para el GPIO " << gpio << std::endl;
            #endif
            return -1;
        }
        return 0;
    }

    int Gpio_t::getNextId() {
        int max_id = -1;
        #ifdef DBG_GPIO
            std::cout << "Gpio_t::getNextId()" << std::endl;
        #endif
        for (const auto& gpioPtr : m_gpio_cfg) {  // Cambiamos a gpioPtr
            if (gpioPtr->ID > max_id) {  // Cambiar a ->
                max_id = gpioPtr->ID;  // Cambiar a ->
            }
        }
        return max_id + 1;
    }

    void Gpio_t::updateGpioMaps() {

        #ifdef DBG_GPIO
            std::cout << "Gpio_t::updateGpioMaps()" << std::endl;
        #endif
        gpioById.clear();
        gpioByPin.clear();
        for (const auto& gpioPtr : m_gpio_cfg) {  // Cambiamos a gpioPtr
            gpioById[gpioPtr->ID] = gpioPtr.get();  // Cambiar a ->
            gpioByPin[gpioPtr->gpio] = gpioPtr.get();  // Cambiar a ->
        }
    }


    void Gpio_t::printGpios() const {
        #ifdef DBG_GPIO
            std::cout << "Gpio_t::printGpios()" << std::endl;
        #endif
        for (const auto& gpioPtr : m_gpio_cfg) {  // Cambiamos el nombre de la variable a gpioPtr
            const GpioConform_t& gpio = *gpioPtr;  // Desreferenciamos el puntero
            std::cout << "ID: " << gpio.ID << ", GPIO: " << gpio.gpio << ", Direction: " << gpio.dir 
                      << ", Edge: " << gpio.edge << ", Value: " << gpio.value << "\n";
        }
    }

    void Gpio_t::addGpio(uint16_t gpio_pin, std::string dir, std::string edge, std::string value) {
        #ifdef DBG_GPIO
            std::cout << "Gpio_t::addGpio" << std::endl;
        #endif

        int id = getNextId();
        auto gpio = std::make_unique<GpioConform_t>(id, gpio_pin, std::move(dir), std::move(edge), std::move(value), true);
        m_gpio_cfg.push_back(std::move(gpio)); // Usa std::move aquí
        gpioById[id] = m_gpio_cfg.back().get(); // Almacena el puntero a la última entrada
        gpioByPin[gpio_pin] = m_gpio_cfg.back().get(); // Almacena el puntero a la última entrada
    }


    const bool Gpio_t::app(bool& flag) 
    {
        //const unsigned int gpio_out = OUT_INTERRUPT;//originalmente es unsigned 
        //const int gpio_in = IN_INTERRUPT;
        struct pollfd fdpoll;
        int m_num_fdpoll { 1 };        
        int m_looper { 0 };
        char *buf[64];

        settings( m_gpio_in  , DIR_IN  ,filenameGpio);
        settings( m_gpio_out , DIR_OUT ,filenameGpio);
        
        gpio_set_edge (m_gpio_in,EDGE_FALLING);
        gpio_set_value(m_gpio_out,VALUE_HIGH);

          
        m_gpio_in_fd = gpio_get_fd_to_value(m_gpio_in);
        // We will wait for button press here for 10s or exit anyway
        if(m_state==true)
        {
        while(m_looper<READING_STEPS) {
            memset((void *)&fdpoll,0,sizeof(fdpoll));
            fdpoll.fd = m_gpio_in_fd;
            fdpoll.events = POLLPRI;
            m_res = poll(&fdpoll,m_num_fdpoll,POLL_TIMEOUT);

            if(m_res < 0) {
                #ifdef DBG_GPIO
                printf("Poll failed...%d\r\n",res);   
                #endif         
                }
            if(m_res == 0) {
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
            ++m_looper;
            fflush(stdout);
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(50));   
        }
        else{            
            gpio_set_value(m_gpio_out,VALUE_HIGH);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));            
        }    
        gpio_set_value(m_gpio_out,VALUE_LOW);
        
   
        return false;
    }


}
