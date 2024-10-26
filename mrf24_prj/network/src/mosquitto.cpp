#include <iostream>
#include <string>

// Se incluyen las bibliotecas estándar necesarias
extern "C" {
    #include <stdio.h>  // Para funciones de entrada/salida
    #include <stdlib.h> // Para funciones de conversión y memoria
}
#include <network/include/mosquitto.h> // Para la biblioteca Mosquitto

namespace MOSQUITTO { // Declaración del espacio de nombres MOSQUITTO


    Mosquitto_t::Mosquitto_t()
    :   user{ MOSQUITTO_USER },   pswd{ MOSQUITTO_PSWD }
    { }

    // Callback que se invoca cuando se establece la conexión con el broker MQTT
    void on_connect(struct mosquitto *mosq, void *obj, int rc) {
        // Imprimir el ID del cliente
        printf("ID: %d\n", * (int *) obj);
        
        // Verificar el código de retorno de la conexión
        if (rc) {
            std::cout << "Error with result code: " << std::to_string(rc) << "n";
            exit(-1); // Salir en caso de error
        }

        // Suscribirse al topic "house/room" con QoS 0
        mosquitto_subscribe(mosq, NULL, "house/room", 0);
    }

    // Callback que se invoca al recibir un nuevo mensaje en un topic suscrito
    void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
        // Imprimir el topic y el contenido del mensaje recibido
        printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
        return;
    }

    // Método para suscribirse a un topic MQTT
    int Mosquitto_t::sub() {
        std::cout << "\033[" << "0" << ";" << "0" << "H"; // Reinicia la posición del cursor en la consola
        int rc, id = 12; // Código de retorno y ID del cliente
        mosquitto_lib_init(); // Inicializa la biblioteca Mosquitto
        
        struct mosquitto *mosq; // Puntero para la instancia de Mosquitto
        mosq = mosquitto_new("subscribe-test", true, &id); // Crea una nueva instancia de Mosquitto
        
        // Establece el nombre de usuario y la contraseña para la conexión
        mosquitto_username_pw_set(mosq, user , pswd );

        // Configura los callbacks para conexión y recepción de mensajes
        mosquitto_connect_callback_set(mosq, on_connect);
        mosquitto_message_callback_set(mosq, on_message);

        // Intentar conectarse al broker MQTT
        rc = mosquitto_connect(mosq, HOST_SERVER_MOSQUITTO, 1883, 10);
        
        // Verificar si la conexión fue exitosa
        if (rc) {
            printf("\t\t\t\t\t\t\tSub : Could not connect to Broker with return code %d\n", rc);
            return -1; // Retornar -1 en caso de error
        }

        // Iniciar el bucle de Mosquitto (comentado)
        // mosquitto_loop_start(mosq);
        // std::cout << "Press Enter to quit...\n";
        // getchar();
        
        // Detener el bucle y desconectar
        mosquitto_loop_stop(mosq, true);
        mosquitto_disconnect(mosq); // Desconectar del broker
        mosquitto_destroy(mosq); // Liberar la memoria de la instancia
        mosquitto_lib_cleanup(); // Limpiar la biblioteca Mosquitto
        return 0; // Retornar 0 para indicar éxito
    }

    // Método para publicar un mensaje en un topic MQTT
    int Mosquitto_t::pub() {
        std::cout << "\033[" << "0" << ";" << "0" << "H"; // Reinicia la posición del cursor en la consola
        static int temperature_day { 11 }; // Temperatura del día (valor inicial)
        static int conter_msj{ 0 }; // Contador de mensajes enviados
        
        // Crear una nueva instancia de Mosquitto para el publicador
        mosq = mosquitto_new("publisher-test", true, NULL);
        
        // Verificar si se creó la instancia correctamente
        if (!mosq) {
            fprintf(stderr, "\t\t\t\t\t\t\t\tError: Out of memory.\n");
            return 1; // Retornar 1 en caso de error
        }

        // Establecer el nombre de usuario y la contraseña para la conexión
        mosquitto_username_pw_set(mosq, "pi", "zero");
        
        // Intentar conectarse al broker MQTT
        rc = mosquitto_connect(mosq, HOST_SERVER_MOSQUITTO, 1883, 60);
        
        // Verificar si la conexión fue exitosa
        if (rc != 0) {
            printf("\n\t\t\t\t\t\t\t\tClient could not connect to broker! Error Code: %d\n", rc);
            mosquitto_destroy(mosq); // Liberar la memoria de la instancia
            return -1; // Retornar -1 en caso de error
        } else {
            printf("\n\t\t\t\t\t\t\t\tPub : We are now connected to the broker!\n");
        }

        // Crear el mensaje a publicar en formato JSON
        const std::string text = "{ temp : " + std::to_string(temperature_day) + " }";
        
        // Publicar el mensaje en el topic "house/room"
        rc = mosquitto_publish(mosq, NULL, "house/room", text.size(), text.data(), 0, false);

        // Verificar si la publicación fue exitosa
        if (rc != 0) {
            fprintf(stderr, "\t\t\t\t\t\t\t\t\tError publishing message! Error Code: %d\n", rc);
        } else {
            printf("\t\t\t\t\t\t\tMessage published successfully!\t\tmsj: %d \n", conter_msj);
        }

        // Desconectar del broker y liberar recursos
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq); // Liberar la memoria de la instancia
        mosquitto_lib_cleanup(); // Limpiar la biblioteca Mosquitto
        conter_msj++; // Incrementar el contador de mensajes
        return temperature_day++; // Retornar el nuevo valor de temperatura
    }

} // Fin del espacio de nombres MOSQUITTO
