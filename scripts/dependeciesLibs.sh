#!/bin/bash


sudo apt update -y
sudo apt upgrade -y

sudo apt install aptitude -y

# Library PNG
sudo apt-get install libpng-dev -y
#sudo apt install libpng16-16 -y
sudo apt-get install zlib1g-dev -y
sudo aptitude install -y libpng-dev

ARCH=$(uname -m)

if [[ $ARCH == "aarch64" ]]; then 
	echo -e " \nes de 64bits\n" 
else 
	echo -e "\nes de 32 bits\n"
fi

if [[ $ARCH == "armv7l" ]]; then 
sudo apt install aptitude -y
apt-cache policy zlib1g
apt-cache policy zlib1g-dev
sudo apt install zlib1g=1:1.2.11.dfsg-1+deb10u2
sudo apt-get install zlib1g-dev --fix-missing
sudo apt install libpng16-16 -y
sudo aptitude install libpng-dev -y

sudo apt install libpng16-16 -ysudo apt clean
sudo apt autoclean
sudo apt autoremove
apt-mark showhold

sudo apt remove zlib1g
sudo apt install zlib1g




cd /tmp
wget http://download.sourceforge.net/libpng/libpng-1.6.39.tar.gz
tar -xzvf libpng-1.6.39.tar.gz
cd libpng-1.6.39
./configure
make
sudo make install
dpkg -l | grep libpng


fi


#install dependencies :
#include <mysql_driver.h>
sudo apt-get install -y libmysqlcppconn-dev

#instal bcm2835 / 64 bits
sudo apt-get install -y libbcm2835-dev

# pip install qrcode

sudo apt-get install qrencode libqrencode-dev -y

sudo apt install libssl-dev -y

sudo apt install libmosquitto-dev -y




cd ~/src

#git clone https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI.git
cd SSD1306_OLED_RPI
#make 
#sudo make install

