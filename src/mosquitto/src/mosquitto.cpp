#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mosquitto/src/mosquitto.h>
#include <app/src/config.h>



//#include <mosquitto.h>


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




void Mosquitto_t::sub() {
	int rc, id=12;
	mosquitto_lib_init();
	struct mosquitto *mosq;
	mosq = mosquitto_new("subscribe-test", true, &id);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);	
	rc = mosquitto_connect(mosq, "raspberry.local", 1883, 10);
	if(rc) {
		printf("Could not connect to Broker with return code %d\n", rc);
		return -1;
	}
	mosquitto_loop_start(mosq);
	printf("Press Enter to quit...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return 0;
}




    void Mosquitto_t::on_connect(struct mosquitto *mosq, void *obj, int rc) {
        printf("ID: %d\n", * (int *) obj);
        if(rc) {
            printf("Error with result code: %d\n", rc);
            exit(-1);
        }
        mosquitto_subscribe(mosq, NULL, "home/room", 0);
    }

    void Mosquitto_t::on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
        printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
    }


}
