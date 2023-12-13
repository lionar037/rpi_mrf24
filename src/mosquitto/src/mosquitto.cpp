#include <iostream>
#include <mosquitto/src/mosquitto.h>
#include <app/src/config.h>

namespace MOSQUITTO{


    void Mosquitto_t::sub(){


    }


    void Mosquitto_t::pub(){

    }

    int Mosquitto_t::init(){
        mosq = mosquitto_new("publisher-test", true, NULL);
        rc = mosquitto_connect(mosq, HOSTNAME_MOSQUITTO , 1883, 60);

                if(rc != 0){
                printf("Client could not connect to broker! Error Code: %d\n", rc);
                mosquitto_destroy(mosq);
                return -1;
        }
        printf("We are now connected to the broker!\n");
        mosquitto_publish(mosq, NULL, "home/room", 6, "is Room", 0, false);
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
    }



    Mosquitto_t::Mosquitto_t()
    //:mosco {std::make_unique<mosquitto>()}
    {
//mosco = mosquitto_new("publisher-test", true, NULL);
        init();
    }
}