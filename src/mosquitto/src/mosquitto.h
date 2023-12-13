#pragma once
#include <memory>
#include <mosquitto.h>


namespace MOSQUITTO{
    struct Mosquitto_t{
            Mosquitto_t();
            ~Mosquitto_t()=default;            
            static void on_connect(struct mosquitto *mosq, void *obj, int rc) ;
            static void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) ;
            int sub();
            int pub();
        private:
            int rc{};
            struct mosquitto * mosq;
            //std::unique_ptr<mosquitto> mosco{};
    };
}