Codigo c++ para raspberry pi y mrf24j40ma
SPI config 

gpio MOSI //master output , slave input
gpio MISO //master input , slave output 
gpio SCK // Clock 
gpio CS //Chip Select
gpio WAKE //Wake
gpio INT //Interrupt
gpio RESET //

# helps:

	http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en535967

commands :

#!/bin/bash

# Obtener la fecha y hora actual en el formato YYYYMMDDHHMM
timestamp=$(date +'%Y%m%d%H%M')

# Construir el mensaje de commit
commit_message="update $timestamp"

# Ejecutar los comandos git
git add . && git commit -m "$commit_message" && git push -u origin master

# Dependencias & Librerias

sudo apt-get install qrencode libqrencode-dev -y
# pip install qrcode

# Library PNG
sudo apt-get install libpng-dev -y

sudo apt-get install zlib1g-dev -y



comunication

		ssh-keygen -R raspberry.local


create comunication secure

		ssh-keygen -t rsa
		ssh-copy-id root@127.0.0.1
		cat ~/.ssh/id_rsa.pub | ssh root@127.0.0.1 'cat >> ~/.ssh/authorized_keys'


 mosquitto_pub -h raspberry.local -p 1883 -t "house/room" -m "{ temp:40 }" -u Username -P "psw"

 mosquitto_sub -h raspberry.local -p 1883 -t "house/room" -v -u Username -P "psw"