#pragma once 
#include <iostream>

#define BCM2708_PERI_BASE 0x3F000000  // Dirección base de los periféricos en una Raspberry Pi 4
#define GPIO_BASE (BCM2708_PERI_BASE + 0x00200000) // Dirección base del registro GPIO

// Definiciones para el manejo de registros GPIO
#define GPFSEL0 0 // Selección de función para los pines 0-9
#define GPSET0 7  // Registro para establecer un pin en 1
#define GPCLR0 10 // Registro para borrar un pin (establecerlo en 0)


namespace GPIO{

    struct Gpio{
        public:
        Gpio();
        ~Gpio();
        uint8_t open_gpio();
        private:
    };

} 