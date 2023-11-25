Codigo c++ para raspberry pi y mrf24j40ma
SPI config 

gpio MOSI //master output , slave input
gpio MISO //master input , slave output 
gpio SCK // Clock 
gpio CS //Chip Select
gpio WAKE //Wake
gpio INT //Interrupt
gpio RESET //

helps:

	http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en535967

commands :

#!/bin/bash

# Obtener la fecha y hora actual en el formato YYYYMMDDHHMM
timestamp=$(date +'%Y%m%d%H%M')

# Construir el mensaje de commit
commit_message="update $timestamp"

# Ejecutar los comandos git
git add . && git commit -m "$commit_message" && git push -u origin master

