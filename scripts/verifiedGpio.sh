#!/bin/bash

# SET=settings
SET=$1

# Verifica si el sistema es de 32 o 64 bits
ARCH=$(uname -m)

if [[ $ARCH == "armv7l" || $ARCH == "arm" ]]; then
    # Sistema de 32 bits
    PINS="16 19 20 21 22 26"
    echo "Sistema de 32 bits detectado."
else
    # Sistema de 64 bits
    PINS="528 531 532 533 534 538"
    echo "Sistema de 64 bits detectado."
fi

# pd = Pull Down , pu = Pull Up 
STATUS_OUTPUT=pd

# op = Out Put , ip = In Put
SET_PIN=ip

# Establece los pines
echo ":$SET:"
if [[ $SET == "settings" || $SET == "set" ]]; then
    for pin in $PINS; do
        if [[ $ARCH == "armv7l" || $ARCH == "arm" ]]; then
            sudo raspi-gpio set $pin $SET_PIN $STATUS_OUTPUT
        else
            # Calcula la resta para los pines de 64 bits
            resta=$(($pin - 512))
            sudo raspi-gpio set $resta $SET_PIN $STATUS_OUTPUT
        fi
    done
else
    echo "No se setearon los pines."
fi

for pin in $PINS; do
        if [[ $ARCH == "armv7l" || $ARCH == "arm" ]]; then
            sudo raspi-gpio set $pin $SET_PIN $STATUS_OUTPUT
        else
            # Calcula la resta para los pines de 64 bits
            resta=$(($pin - 512))
            sudo raspi-gpio set $resta $SET_PIN $STATUS_OUTPUT
                echo "PIN :  $resta"
        fi
done


GPIO=524  # Puedes cambiar este número según sea necesario

# Función para configurar GPIO
config_gpio() {
    local direction=$1
    echo "$direction" | sudo tee /sys/class/gpio/gpio$GPIO/direction
}

# Función para establecer valor en GPIO
set_gpio_value() {
    local value=$1
    local result=$(echo "$value" | sudo tee /sys/class/gpio/gpio$GPIO/value)
    echo "GPIO $GPIO resultado ($value): $result"
}

# Función para establecer edge en GPIO
set_gpio_edge() {
    local edge=$1
    local result=$(echo "$edge" | sudo tee /sys/class/gpio/gpio$GPIO/edge)
    echo "$edge edge result: $result"
}

# Configuración de GPIO
echo "Configurando GPIO $GPIO como salida"
config_gpio "out"
set_gpio_value 1
set_gpio_value 0

echo "..............."

echo "Configurando GPIO $GPIO como entrada"
config_gpio "in"
set_gpio_edge "rising"
set_gpio_edge "falling"