#!/bin/bash


sudo apt update -y
sudo apt upgrade -y

# Library PNG
sudo apt-get install libpng-dev -y

sudo apt-get install zlib1g-dev -y

#install dependencies :
#include <mysql_driver.h>
sudo apt-get install -y libmysqlcppconn-dev

#instal bcm2835 / 64 bits
sudo apt-get install -y libbcm2835-dev

do apt-get install qrencode libqrencode-dev -y
# pip install qrcode





cd ~/src

#git clone https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI.git
cd SSD1306_OLED_RPI
#make 
#sudo make install

