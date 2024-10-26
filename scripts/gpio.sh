#!/bin/bash

# Asegúrate de que el script se ejecute con permisos de superusuario
# para poder escribir en /sys/class/gpio/export

SET=$1

ARCH=$(uname -m)

echo "Insertar ./ejecutable + cmd"
echo "Options: \n \t\t enable_gpio, set, chmod, debbug, info, list, output_low"
echo "./configGpioSuccess.sh enable_gpio"
echo "./configGpioSuccess.sh set"
echo "./configGpioSuccess.sh chmod"
echo "./configGpioSuccess.sh debbug"
echo "./configGpioSuccess.sh info"
echo "./configGpioSuccess.sh list"
echo "./configGpioSuccess.sh output_low"


if [[ $ARCH == "armv7l" || $ARCH == "arm" ]]; then
    INIT=0
    END=53
    #INIT=524
    #END=569
else
    #INIT=524
    #END=569
    INIT=0
    END=53
fi



FROM_TO="$INIT..$END"

if [[ $SET == "set" ]]; then
    for PIN in $(seq $INIT $END); do
        if [[ -e /sys/class/gpio/gpio$PIN ]]; then
            echo "GPIO $PIN ya existe"
        else
            RESULT=$(echo $PIN | sudo tee /sys/class/gpio/export > /dev/null)
            echo "$RESULT : $PIN"
        fi
    done
fi

if [[ $SET == "info" ]]; then
    gpioinfo
    echo " "
    ls -1l /sys/class/gpio
fi

RESULT1=$(($INIT - 512))
RESULT2=$(($END - 512))

if [[ $INIT -gt 500 ]]; then
    PIN_DBG="$RESULT1..$RESULT2"
else
    PIN_DBG="$FROM_TO"
fi

if [[ $SET == "list" ]]; then
    echo "$PIN_DBG : $FROM_TO"
    if [[ $INIT -gt 500 ]]; then
        for PIN in $(seq $RESULT1 $RESULT2); do
            sudo raspi-gpio get $PIN
        done
    else
        for PIN in $(seq $INIT $END); do
            sudo raspi-gpio get $PIN
        done
    fi
fi

if [[ $SET == "chmod" ]]; then
    sudo chgrp gpio /sys/class/gpio/export
    sudo chgrp gpio /sys/class/gpio/unexport
    sudo chmod 660 /sys/class/gpio/export
    sudo chmod 660 /sys/class/gpio/unexport
    SET="debbug"
fi

if [[ $SET == "enable_gpio" ]]; then
    for PIN in $(seq $INIT $END); do
        echo "Habilitando GPIO pin: $PIN"
        sudo chgrp gpio /sys/class/gpio/gpio$PIN/value
        sudo chgrp gpio /sys/class/gpio/gpio$PIN/direction
        sudo chmod 660 /sys/class/gpio/gpio$PIN/value
        sudo chmod 660 /sys/class/gpio/gpio$PIN/direction
    done
fi

if [[ $SET == "debbug" ]]; then
    sudo ls -l /sys/class/gpio
    sudo cat /sys/kernel/debug/gpio
fi

# Pines que no deben ser modificados porque son usados por SPI, UART, I2C, USB
RESERVED_GPIO=(2 3 9 10 11 14 15 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 44 45 46 47)

if [[ $SET == "output_low" ]]; then
    # Función para verificar si un pin está en la lista de reservados
    is_reserved() {
        local pin=$1
        for reserved in "${RESERVED_GPIO[@]}"; do
            if [[ "$pin" -eq "$reserved" ]]; then
                return 0  # El pin está reservado
            fi
        done
        return 1  # El pin no está reservado
    }

    # Configurar todos los GPIO disponibles como salida y en estado bajo
    for pin in $(seq 0 27); do
        if ! is_reserved "$pin"; then
            echo "Configurando GPIO $pin como salida (estado bajo)..."
            sudo raspi-gpio set $pin op
            sudo raspi-gpio set $pin lo
        else
            echo "GPIO $pin está reservado y no será modificado."
        fi
    done

    echo "Todos los GPIO configurados."
fi


ls /sys/class/gpio/