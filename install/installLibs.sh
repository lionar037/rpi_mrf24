#!/bin/bash

sudo apt update -y

sudo apt upgrade -y

sudo apt install libmosquitto-dev -y

sudo apt-get install qrencode libqrencode-dev -y

sudo apt-get install libpng-dev -y

sudo apt-get install zlib1g-dev -y

sudo apt install mosquitto mosquitto-clients -y

sudo apt install libmariadb-dev-compat libmariadb-dev -y

sudo apt install mysql-server -y

sudo systemctl enable mosquitto

sudo systemctl start mosquitto

#sudo nano /etc/mosquitto/mosquitto.conf

#Enable Security:

#password_file /etc/mosquitto/passwd
#allow_anonymous false


#Create Passwords:

#sudo mosquitto_passwd -c /etc/mosquitto/passwd myUsers

#Reset:

#sudo systemctl restart mosquitto


###!/bin/bash
cd ~/Downloads

wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz

tar zxvf bcm2835-1.71.tar.gz

cd bcm2835-1.71

./configure
make
sudo make check
sudo make install

rm -Rf  ~/Downloads/*

ls /usr/local/include/bcm2835.h

cd ~/src
git clone https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI.git

cd SSD1306_OLED_RPI-1.6.1
make
sudo make install


cd ~/src


#sudo systemctl status mysql

#sudo mysql_secure_installation

#sudo mysql -u root -p

