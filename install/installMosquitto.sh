#!/bin/bash


sudo apt list --upgradable

sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
sudo apt update

sudo apt install mosquitto mosquitto-clients -y

sudo cat /etc/mosquitto/mosquitto.conf

#Asegúrate de descomentar o agregar la línea:
#listener 1883

#Reiniciar Mosquitto para aplicar los cambios
sudo systemctl restart mosquitto