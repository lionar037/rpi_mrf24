//#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <gpio.h>

namespace GPIO{

 uint8_t Gpio::open_gpio () {
    // Abre el archivo /dev/mem para acceder a los registros GPIO
    int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd < 0) {
        std::cerr << "No se pudo abrir /dev/mem. Asegúrate de tener permisos suficientes." << std::endl;
        return -1;
        //return 0x00;
    }

    // Mapea la dirección base de los registros GPIO a la memoria
    void* gpio_map = mmap(
        NULL,
        4096,  // Tamaño de la página
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        mem_fd,
        GPIO_BASE
    );

    if (gpio_map == MAP_FAILED) {
        std::cerr << "Error al mapear la dirección de los registros GPIO." << std::endl;
        return -1;
    }

    // Configura el pin xx (GPIO xx) como salida
    unsigned int* gpio = (unsigned int*)gpio_map;
    gpio[GPFSEL0] |= (1 << 21); // Establece el modo del pin 17 a 1 (salida)

    // Establece el pin xx en 1 (alto)
    gpio[GPSET0] = (1 << 21);

    // Puedes establecer el pin xx en 0 (bajo) de la siguiente manera:
    // gpio[GPCLR0] = (1 << 21);

    // Desmapea la memoria y cierra el archivo /dev/mem
    munmap(gpio_map, 4096);
    close(mem_fd);

    return 0x00;
}


    Gpio::Gpio () {
    std::cout<<"Constructor gpio\r\n";
    }

    Gpio::~Gpio () {
    std::cout<<"destructor gpio\r\n";
    }
 }