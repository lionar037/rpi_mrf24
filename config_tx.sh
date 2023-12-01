#!/bin/bash

# Verifica si se proporcionó al menos un argumento
if [ $# -lt 1 ]; then
    echo "Uso: $0 <config>"
    exit 1
fi

# Lee el primer argumento pasado al script
config=$1

# Realiza acciones en función del argumento
case $config in
    rx)
        echo "Ejecutando con rx..."
        # Agrega aquí las acciones específicas para config1
        echo "Configuring as a receiver"
        cp libs/app/config.h libs/app/config.h.bkp 

        sed -i 's,//#define USE_MRF24_TX,#define USE_MRF24_TX,g' libs/app/config.h
        sed -i 's,#define USE_MRF24_RX,//#define USE_MRF24_RX,g' libs/app/config.h

        echo "Configure as receiver Rx ..."
        ;;
    tx)
        #echo "Ejecutando con tx..."
        echo "Configuring as a transmitter"
        # Agrega aquí las acciones específicas para config2
        sed -i 's,#define USE_MRF24_TX,//#define USE_MRF24_TX,g' libs/app/config.h
        sed -i 's,//#define USE_MRF24_RX,#define USE_MRF24_RX,g' libs/app/config.h
        echo "Configure as Transmitter Tx ..."
        ;;
    *)
        echo "No hay cambios : $config"
        exit 1
        ;;
esac

# El resto del script aquí...

# Ejemplo: Mostrar el contenido del archivo de configuración
echo "Contenido de $config:"
cat "$config"
