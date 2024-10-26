#pragma once
#include <memory>
#include <mosquitto.h>
#include <app/include/config.h>

#ifdef ENABLE_MOSQUITTO    
    //#redefine HOST_SERVER_MOSQUITTO "192.168.1.41"
    //#redefine HOST_SERVER_MOSQUITTO "raspberry.local"
    //#redefine HOST_SERVER_MOSQUITTO "mosquitto.local"
#else 
    #define HOST_SERVER_MOSQUITTO "mosquitto.local"
#endif

//configuracion de mosquitto
//sudo nano /etc/mosquitto/mosquitto.conf 
//#allow_anonymous true
//#listener 1883 0.0.0.0
//#password_file /etc/mosquitto/passwd

#define MOSQUITTO_USER "pi"
#define MOSQUITTO_PSWD "zero"

namespace MOSQUITTO{
    struct Mosquitto_t{
            explicit Mosquitto_t();
            ~Mosquitto_t()=default;                        
            int sub();
            int pub();
        private:
            int rc{};
            struct mosquitto * mosq;   
            const std::string user;   
            const std::string pswd;
    };
}