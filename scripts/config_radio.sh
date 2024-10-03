#!/bin/bash

# Verifica si se proporcionó al menos un argumento
if [ $# -lt 1 ]; then
    echo "Uso: $0 <config>"
    exit 1
fi

# Lee el primer argumento pasado al script
config=$1

arch=$(uname -m)


# Modifica el Makefile en función de la arquitectura del sistema
if [ "$arch" == "x86_64" ] || [ "$arch" == "aarch64" ]; then
    echo "Sistema de 64 bits detectado, comentando la línea en el Makefile"
    sed -i 's,LIBS += -lSSD1306_OLED_RPI,#LIBS += -lSSD1306_OLED_RPI ,g' Makefile
elif [ "$arch" == "i386" ] || [ "$arch" == "i686" ] || [ "$arch" == "armv7l" ]; then
    echo "Sistema de 32 bits detectado, descomentando la línea en el Makefile"
    sed -i 's,#LIBS += -lSSD1306_OLED_RPI,LIBS += -lSSD1306_OLED_RPI,g' Makefile
else
    echo "Arquitectura no soportada: $arch"
    exit 1
fi



# Realiza acciones en función del argumento
case $config in
    tx)
        # Agrega aquí las acciones específicas para config1
        echo "Configuring as a transmitter"
        #cp libs/app/src/config.h libs/app/src/config.h.bkp 
        sed -i 's,//#define USE_MRF24_TX,#define USE_MRF24_TX,g' src/app/src/config.h
        sed -i 's,#define USE_MRF24_RX,//#define USE_MRF24_RX,g' src/app/src/config.h

#        sed -i 's,LIBS += -lSSD1306_OLED_RPI,#LIBS += -lSSD1306_OLED_RPI ,g' Makefile


        echo "Configure as transmitter Rx ..."
        ;;
    rx)
        echo "Configuring as a receiver"
        # Agrega aquí las acciones específicas para config2
        sed -i 's,#define USE_MRF24_TX,//#define USE_MRF24_TX,g' src/app/src/config.h
        sed -i 's,//#define USE_MRF24_RX,#define USE_MRF24_RX,g' src/app/src/config.h
        echo "Configure as receiver Tx ..."
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
