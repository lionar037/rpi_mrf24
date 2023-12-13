#pragma once
#include <mosquitto.h>
#include <memory>

namespace MOSQUITTO{
    struct Mosquitto_t{
            Mosquitto_t();
            ~Mosquitto_t()=default;
            void sub();
            void pub();
            int init();
        private:
            int rc{};
            struct mosquitto * mosq;
            //std::unique_ptr<mosquitto> mosco{};
    };
}