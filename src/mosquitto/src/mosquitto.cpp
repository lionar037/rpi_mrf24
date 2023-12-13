#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mosquitto/src/mosquitto.h>
#include <app/src/config.h>


namespace MOSQUITTO{

    void on_connect(struct mosquitto *mosq, void *obj, int rc) {
        printf("ID: %d\n", * (int *) obj);
        if(rc) {
            printf("Error with result code: %d\n", rc);
            exit(-1);
        }
        mosquitto_subscribe(mosq, NULL, "house/room", 0);        
    }



    void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
        printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
    }


    int Mosquitto_t::sub() {
    	int rc, id=12;
    	mosquitto_lib_init();
    	struct mosquitto *mosq;
    	mosq = mosquitto_new("subscribe-test", true, &id);
    	mosquitto_connect_callback_set(mosq, on_connect);
    	mosquitto_message_callback_set(mosq, on_message);	
    	rc = mosquitto_connect(mosq, "HOSTNAME_MOSQUITTO", 1883, 10);
        mosquitto_username_pw_set(mosq, "pi", "zero");
    	if(rc) {
    		printf("\t\tSub \tCould not connect to Broker with return code %d\n", rc);
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

    int Mosquitto_t::pub(){
        mosq = mosquitto_new("publisher-test", true, NULL);
        rc = mosquitto_connect(mosq, HOSTNAME_MOSQUITTO , 1883, 60);
        mosquitto_username_pw_set(mosq, "pi", "zero");

                if(rc != 0){
                printf("Client could not connect to broker! Error Code: %d\n", rc);
                mosquitto_destroy(mosq);
                return -1;
        }
        printf("\t\tPub  \tWe are now connected to the broker!\n");
        mosquitto_publish(mosq, NULL, "house/room", 6, " is Room ", 0, false);
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 0;
    }

}
