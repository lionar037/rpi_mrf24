#pragma once
#include <memory>
#include <mosquitto.h>
#include <app/include/config.h>

#ifdef ENABLE_MOSQUITTO    
    //#redefine HOST_SERVER_MOSQUITTO "192.168.1.38"
#else 
    #define HOST_SERVER_MOSQUITTO "mosquitto.local"
#endif


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