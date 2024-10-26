#!/bin/bash


sudo apt update -y
sudo apt upgrade -y
sudo aptitude install -y  curl
sudo apt install aptitude -y

# Library PNG
sudo apt-get install zlib1g-dev -y
sudo aptitude install -y libpng-dev
sudo apt install libmosquitto-dev -y

sudo apt-get install qrencode libqrencode-dev -y

sudo apt-get install libpng-dev -y

sudo apt-get install zlib1g-dev -y

sudo apt install mosquitto mosquitto-clients -y

sudo apt install libmariadb-dev-compat libmariadb-dev -y

sudo apt install mysql-server -y

sudo systemctl enable mosquitto

sudo systemctl start mosquitto

#security
sudo apt-get install -y libssl-dev

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


#install dependencies : #include <mysql_driver.h>
sudo apt-get install -y libmysqlcppconn-dev

if [[ $ARCH == "aarch64" ]]; then
	echo "instal bcm2835 / 64 bits"
	sudo apt-get install -y libbcm2835-dev
else
	cd /tmp
	#cd ~
	wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.68.tar.gz  # Cambia a la última versión si es necesario
	tar xzf bcm2835-1.68.tar.gz
	cd bcm2835-1.68
	./configure
	make
	sudo make install
fi
# pip install qrcode

sudo apt-get install qrencode libqrencode-dev -y

#sudo apt install libssl-dev -y

sudo apt install libmosquitto-dev -y



if [[ $APP == "oled" ]]; then
	cd ~/src
	git clone https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI.git
	cd ~/src/SSD1306_OLED_RPI
	make 
	sudo make install

	#git clone https://github.com/gavinlyonsrepo/Display_Lib_RPI.git
	#cd Display_Lib_RPI.git
	#make
	#sudo make install
fi