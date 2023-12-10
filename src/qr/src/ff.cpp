#include <iostream>
#include <cstdlib>
#include <ctime>
#include <qr/src/qr.h>
extern "C"{
    #ifdef _WIN32
    #include <conio.h>
    #else
    #include <unistd.h>
    #endif
}


namespace QR{

    void Qr_img_t::drawRectangle(int width, int height) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                char randomChar = rand() % 26 + 'A';  // Generar un carácter aleatorio (A-Z)
                std::cout << randomChar;
            }
            std::cout << std::endl;
        }

        // Posicionar el cursor al inicio de la impresión
    #ifdef _WIN32
        std::cout << "Presiona Enter para imprimir otro cuadrado...";
        _getch();  // Esperar a que se presione una tecla en Windows
    #else
        std::cout << "\033[" << 30 << "A";
        usleep(500000);  // Esperar medio segundo en sistemas basados en Unix
    #endif
    }

    bool Qr_img_t::create2(const std::string_view& createView) {
        int width = 50;
        int height = 10;

        // Inicializar la semilla para obtener valores aleatorios diferentes en cada ejecución
        std::srand(std::time(0));

        for (int i = 0; i < 10; i++) {
            drawRectangle(width, height);
        }
        return false;
    }
}