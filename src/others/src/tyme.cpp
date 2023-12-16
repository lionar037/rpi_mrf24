//#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <app/src/config.h>
#include <others/src/tyme.h>

//#include <unistd.h> // Libreria para usleep

namespace TYME{

    Time_t::Time_t(){
        #ifdef DBG
            std::cout << " Time_t()\n";
        #endif

    }

    Time_t::~Time_t(){
        #ifdef DBG
            std::cout << " ~Time_t()\n";
        #endif
    }

    void Time_t::delay_ms(const uint32_t t){
        std::chrono::microseconds delay_loc(t);
        std::this_thread::sleep_for(delay_loc);
        //     usleep(t);//otra forma
    return;
    }

    const std::string  Time_t::get_tyme()
    {

        auto now = std::chrono::system_clock::now();

        // Convertir el tiempo actual a una estructura de tiempo local
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&currentTime);

        // Formatear la fecha y hora según tu especificación
        std::ostringstream oss;
            std::cout<<"\n";
        oss << std::put_time(&localTime, "%Y%m%d%H%M%S");
            std::cout<<"\n";
        std::string tyme = oss.str();
        return tyme;
    }





//#include <string_view>

bool Time_t::obtenerHoraNTP(const std::string_view& servidorNTP, std::chrono::system_clock::time_point& resultado) {
    const int puertoNTP = 123;
    const int tamanoPaqueteNTP = 48;

    // Obtener información sobre el servidor NTP
    addrinfo hints, *res;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    if (getaddrinfo(servidorNTP.data(), std::to_string(puertoNTP).c_str(), &hints, &res) != 0) {
        perror("Error al obtener información del servidor NTP");
        return false;
    }

    int socketNTP = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socketNTP == -1) {
        perror("Error al crear el socket");
        freeaddrinfo(res);
        return false;
    }

    char paqueteNTP[tamanoPaqueteNTP];
    std::memset(paqueteNTP, 0, sizeof(paqueteNTP));
    paqueteNTP[0] = 0x1B;

    if (sendto(socketNTP, paqueteNTP, sizeof(paqueteNTP), 0, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Error al enviar la solicitud al servidor NTP");
        close(socketNTP);
        freeaddrinfo(res);
        return false;
    }

    char respuestaPaquete[tamanoPaqueteNTP];
    if (recv(socketNTP, respuestaPaquete, sizeof(respuestaPaquete), 0) == -1) {
        perror("Error al recibir la respuesta del servidor NTP");
        close(socketNTP);
        freeaddrinfo(res);
        return false;
    }

    close(socketNTP);
    freeaddrinfo(res);

    uint64_t segundos = (static_cast<uint8_t>(respuestaPaquete[40]) << 24) |
                        (static_cast<uint8_t>(respuestaPaquete[41]) << 16) |
                        (static_cast<uint8_t>(respuestaPaquete[42]) << 8) |
                        static_cast<uint8_t>(respuestaPaquete[43]);

    uint64_t fracciones = (static_cast<uint8_t>(respuestaPaquete[44]) << 24) |
                           (static_cast<uint8_t>(respuestaPaquete[45]) << 16) |
                           (static_cast<uint8_t>(respuestaPaquete[46]) << 8) |
                           static_cast<uint8_t>(respuestaPaquete[47]);

    if (fracciones == 0) {
        std::cerr << "Error: Fracciones de tiempo NTP iguales a cero." << std::endl;
        return false;
    }

    auto tiempoNTP = std::chrono::system_clock::time_point(std::chrono::seconds(segundos) + std::chrono::microseconds(fracciones * 1000000ULL / (1ULL << 32)));

    // Ajustar a la zona horaria de Argentina (GMT-3)
    auto ajusteZonaHoraria = std::chrono::hours(-3);
    resultado = tiempoNTP + ajusteZonaHoraria;

    return true;
}

int Time_t::timeUpdate() {
    const std::string servidorNTP = "pool.ntp.org";

    // Obtener la hora actual local
    auto tiempoLocal = std::chrono::system_clock::now();

    std::chrono::system_clock::time_point tiempoNTP;
    if (obtenerHoraNTP(servidorNTP, tiempoNTP)) {
        // Ajustar a la zona horaria de Argentina (GMT-3)
        auto ajusteZonaHoraria = std::chrono::hours(-3);
        //tiempoNTP += ajusteZonaHoraria;
	//tiempoActual += ajusteZonaHoraria;

tiempoLocal+=ajusteZonaHoraria;

	std::time_t tiempoActual = std::chrono::system_clock::to_time_t(tiempoLocal);
        std::cout << "La hora actual en Argentina (obtenida de " << servidorNTP << ") es: "
                  << std::put_time(std::localtime(&tiempoActual), "%c") << std::endl;

        std::time_t tiempoNTP_t = std::chrono::system_clock::to_time_t(tiempoNTP);
        std::cout << "La hora obtenida del servidor NTP es: "
                  << std::put_time(std::localtime(&tiempoNTP_t), "%c") << std::endl;
    } else {
        std::cerr << "No se pudo obtener la hora del servidor NTP." << std::endl;
    }

    return 0;
}
}