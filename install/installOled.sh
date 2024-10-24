#!/bin/bash

#cd ~/Downloads
cd ~/src

git clone https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI.git 
#git clone https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI.git SSD1306_OLED

#cd SSD1306_OLED

cd SSD1306_OLED_RPI-1.6.1
make
sudo make install
