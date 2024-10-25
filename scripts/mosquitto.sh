#!/bin/bash

MOSQ=$1

# Verificar el argumento
if [[ -z "$MOSQ" ]]; then
    echo "Uso: $0 {status|start|stop|restart|enable}"
    exit 1
fi

# Comando para gestionar el servicio Mosquitto
case $MOSQ in
    status)
        sudo systemctl status mosquitto
        ;;
    start)
        sudo systemctl start mosquitto
        ;;
    stop)
        sudo systemctl stop mosquitto
        ;;
    restart)
        sudo systemctl restart mosquitto
        ;;
    enable)
        sudo systemctl enable mosquitto
        ;;
    *)
        echo "Opción no válida. Usa: status, start, stop, restart."
        exit 1
        ;;
esac
