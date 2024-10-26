//////////////////////////////////////////////////////////////////////////////
//     
//          filename            :   file.cpp
//          License             :   GNU 
//          Author              :   Lio
//          Change History      :
//          Processor           :   ARM
//          Hardware            :		
//          Compiler            :   ARM
//          Company             :   lionar037
//          Dependencies        :   gnu
//          Description         :   Implementación de funciones para la gestión 
//                                de archivos binarios. Incluye la carga de 
//                                archivos y la creación de nuevos archivos 
//                                con datos específicos.
//          @brief               :   Esta clase proporciona métodos para 
//                                cargar datos desde archivos binarios y 
//                                crear archivos nuevos con un nombre de 
//                                archivo basado en la fecha y hora actual.
// 
//////////////////////////////////////////////////////////////////////////////

#include <iostream> // Para la entrada/salida estándar
#include <fstream> // Para operaciones de archivo

#include <iomanip> // Para manipuladores de flujo de formato
#include <chrono> // Para el manejo del tiempo
#include <sstream> // Para la clase std::ostringstream

// Incluir bibliotecas C estándar
extern "C"
{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdint.h>
}

// Incluir archivos de cabecera personalizados
#include <files/include/file.h> // Archivo de cabecera para la clase File_t
#include <app/include/config.h> // Configuraciones específicas de la aplicación
#include <others/include/color.h> // Para colores en la salida

namespace FILESYSTEM { // Declaración del espacio de nombres FILESYSTEM

    // Constructor de la clase File_t
    File_t::File_t()
    :   m_filename  { LOG_FILENAME }, // Inicializar el nombre del archivo con LOG_FILENAME
        m_buffer    { "@ABCDEF" }, // Inicializar el buffer con datos predeterminados
        m_size_data { m_buffer.size() * sizeof(char) } // Calcular el tamaño de los datos
    {
        #ifdef DBG // Si se está en modo depuración
            std::cout << "File_t::File_t()\n"; // Mensaje de depuración
        #endif
    }

    // Destructor de la clase File_t
    File_t::~File_t(){
        #ifdef DBG // Si se está en modo depuración
            std::cout << "~File_t()\n"; // Mensaje de depuración
        #endif
    }

    // Método para cargar un archivo binario y retornar sus datos como puntero a unsigned char
    unsigned char* File_t::loadFile(const std::string_view filename) {
        // Crear un flujo de entrada para abrir el archivo en modo binario
        std::ifstream file(filename.data(), std::ios::binary);

        // Verificar si el archivo se abrió correctamente
        if (!file.is_open()) {
            #ifdef DBG_FILES // Si se está en modo depuración de archivos
                std::cerr << "Error al abrir el archivo: " << filename.data() << std::endl; // Mensaje de error
            #endif
            return nullptr; // Retornar nullptr si no se pudo abrir
        }

        packet_mrf24 packet; // Declarar un paquete para almacenar datos leídos

        // Leer el paquete del archivo
        file.read(reinterpret_cast<char*>(&packet), sizeof(packet_mrf24));

        // Comprobar la dirección MAC del receptor
        const auto& address_rx = packet.mac_address_rx;

        if (address_rx != ADDRESS_LONG_SLAVE) { // Verificar si la dirección es válida
            #ifdef DBG_FILES
                std::cerr << "Es un MRF24 no válido." << std::endl; // Mensaje de error
            #endif        
            file.close(); // Cerrar el archivo
            return nullptr; // Retornar nullptr
        }

        size_t dataSize = packet.size; // Obtener el tamaño de los datos del paquete

        // Asignar memoria para los datos de imagen
        unsigned char* imgdata = new unsigned char[dataSize];

        // Leer los datos de la imagen del archivo
        file.read(reinterpret_cast<char*>(imgdata), dataSize);

        // Verificar el PAN ID
        if (packet.panid != PAN_ID) {
            #ifdef DBG_FILES
                std::cerr << SET_COLOR_RED_TEXT << "PAN_ID no válido." << std::endl; // Mensaje de error
            #endif        
            file.close(); // Cerrar el archivo
            return nullptr; // Retornar nullptr
        }

        file.close(); // Cerrar el archivo
        return imgdata; // Retornar los datos leídos
    }

    // Método para crear un nuevo archivo con datos específicos
    bool File_t::create(const std::string_view& tmp) {
        const std::string name_files = "log/" + m_filename + tyme() + ".bin"; // Construir el nombre del archivo
        std::ofstream file(name_files, std::ios::binary); // Abrir el archivo en modo binario

        // Verificar si el archivo se abrió correctamente
        if (file.is_open()) {
            const auto bufferSize = strlen(tmp.data()); // Obtener el tamaño del buffer
            file.write(tmp.data(), bufferSize); // Escribir datos en el archivo

            // Cerrar el archivo
            file.close();
            #ifdef DBG_FILES
                std::cout << "\nData written to the file successfully." << std::endl; // Mensaje de éxito
            #endif
            return true; // Retornar verdadero si se creó el archivo
        } else {
            std::cerr << "\nNot open file." << std::endl; // Mensaje de error si no se pudo abrir
            return false; // Retornar falso
        }

        return false; // Retornar falso por defecto
    }

    // Método para obtener la fecha y hora actual como una cadena formateada
    const std::string File_t::tyme() {
        auto now = std::chrono::system_clock::now(); // Obtener el tiempo actual

        // Convertir el tiempo actual a una estructura de tiempo local
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&currentTime);

        // Formatear la fecha y hora según tu especificación
        std::ostringstream oss; // Crear un objeto ostringstream para el formato
        std::cout << "\n"; // Nueva línea en la salida estándar
        oss << std::put_time(&localTime, "%Y%m%d%H%M%S"); // Formatear la fecha y hora
        std::cout << "\n"; // Nueva línea en la salida estándar
        std::string tyme = oss.str(); // Obtener la cadena formateada
        return tyme; // Retornar la cadena
    }

}
