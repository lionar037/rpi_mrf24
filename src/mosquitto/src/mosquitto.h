#pragma once
#include <memory>
#include <mosquitto.h>

#define HOSTNAME_MOSQUITTO "raspberry-2.local"

//#define HOSTNAME_MOSQUITTO "192.168.1.38"

namespace MOSQUITTO{
    struct Mosquitto_t{
            Mosquitto_t();
            ~Mosquitto_t()=default;            
            // void on_connect(struct mosquitto *mosq, void *obj, int rc) ;
            // void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) ;
            int sub();
            int pub();
        private:
            int rc{};
            struct mosquitto * mosq;
            //std::unique_ptr<mosquitto> mosco{};
    };
}