#pragma once
#include <mosquitto.h>
#include <memory>

namespace MOSQUITTO{
    struct Mosquitto_t{
            Mosquitto_t();
            ~Mosquitto_t()=default;
            void sub();
            void pub();
            void init();
        private:
            struct mosquitto * mosq;
            //std::unique_ptr<mosquitto> mosco{};
    };
}