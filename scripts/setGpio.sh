#!/bin/bash
ARCH=$(uname -m)
# Definir el rango de pines
if [[ $ARCH == "armv7l" || $ARCH == "arm" ]]; then
    PINS=$(seq 0 30)
else
    PINS=$(seq 512 542)
fi

# Setear los pines como salidas y en alto
for pin in $PINS; do
    sudo raspi-gpio set $pin op dh
    sudo raspi-gpio get $pin
    sleep 0.2
done

 sudo raspi-gpio set 16 ip pu
# Configurar los pines en alto (ya se hizo en el bucle anterior, pero puedes opcionalmente repetirlo)
for pin in $PINS; do
    	sudo raspi-gpio get $pin
	sleep 0.2
done

#sudo raspi-gpio get

echo "Pines del $PINS configurados como salida y en alto."
