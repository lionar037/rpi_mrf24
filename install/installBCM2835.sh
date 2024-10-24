#!/bin/bash
cd Downloads

wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz

tar zxvf bcm2835-1.71.tar.gz

cd bcm2835-1.71

./configure
make
sudo make check
sudo make install

ls /usr/local/include/bcm2835.h
