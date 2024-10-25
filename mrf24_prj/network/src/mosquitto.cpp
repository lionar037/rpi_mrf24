#include <iostream>
#include <string>

extern "C"{
    #include <stdio.h>
    #include <stdlib.h>
}
#include <network/include/mosquitto.h>
//#include <app/src/config.h>


namespace MOSQUITTO{

    void on_connect(struct mosquitto *mosq, void *obj, int rc) {
        printf("ID: %d\n", * (int *) obj);
        if(rc) {
            std::cout<<"Error with result code: "<< std::to_string (rc) <<"n";
            exit(-1);
        }
        mosquitto_subscribe(mosq, NULL, "house/room", 0);        
    }



    void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
        printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
        return;
    }


    int Mosquitto_t::sub() {
        std::cout << "\033[" << "0" << ";" << "0" << "H" ;
    	int rc, id=12;
    	mosquitto_lib_init();
    	struct mosquitto *mosq;
    	
        mosq = mosquitto_new("subscribe-test", true, &id);
    	mosquitto_username_pw_set(mosq, "pi", "zero");
        mosquitto_connect_callback_set(mosq, on_connect);
    	mosquitto_message_callback_set(mosq, on_message);	
    	rc = mosquitto_connect(mosq, HOST_SERVER_MOSQUITTO, 1883, 10);
        
    	if(rc) {
    		printf("\t\t\t\t\t\t\tSub : Could not connect to Broker with return code %d\n", rc);
    		return -1;
    	}
    	//mosquitto_loop_start(mosq);
    	//std::cout<<"Press Enter to quit...\n";
    	//getchar();
    	mosquitto_loop_stop(mosq, true);
    	mosquitto_disconnect(mosq);
    	mosquitto_destroy(mosq);
    	mosquitto_lib_cleanup();
    	return 0;
    }



    int Mosquitto_t::pub(){
        std::cout << "\033[" << "0" << ";" << "0" << "H" ;
        static int temperature_day { 11 };
        static int conter_msj{0};
        
        mosq = mosquitto_new("publisher-test", true, NULL);
    if (!mosq) {
        fprintf(stderr, "\t\t\t\t\t\t\t\tError: Out of memory.\n");
        return 1;
    }



        mosquitto_username_pw_set(mosq, "pi", "zero");

        rc = mosquitto_connect(mosq, HOST_SERVER_MOSQUITTO , 1883, 60);
        
        if(rc != 0){
            printf("\t\t\t\t\t\t\t\t\tClient could not connect to broker! Error Code: %d\n", rc);
            mosquitto_destroy(mosq);
            return -1;
        }else{
        printf("\t\t\t\t\t\t\t\t\tPub : We are now connected to the broker!\n");
        }

        const std::string text= "{ temp : " + std::to_string(temperature_day) +" }";
        
        rc = mosquitto_publish(mosq, NULL, "house/room", text.size() ,text.data() , 0, false);

        if (rc != 0) {
            fprintf(stderr, "\t\t\t\t\t\t\t\t\tError publishing message! Error Code: %d\n", rc);
            } else {
                printf("\t\t\t\t\t\t\tMessage published successfully!\t\tmsj: %d \n",conter_msj);
            }

        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        conter_msj++;
        return temperature_day++;
    }

}
