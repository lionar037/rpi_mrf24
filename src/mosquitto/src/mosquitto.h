#pragma once
#include <memory>
#include <mosquitto.h>

#define HOST_SERVER_MOSQUITTO "raspberry-2.local"

//#define HOST_SERVER_MOSQUITTO "192.168.1.38"

namespace MOSQUITTO{
    struct Mosquitto_t{
            Mosquitto_t()=default;
            ~Mosquitto_t()=default;                        
            int sub();
            int pub();
        private:
            int rc{};
            struct mosquitto * mosq;      
    };
}