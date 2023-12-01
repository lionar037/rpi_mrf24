#!/bin/bash

echo "Configuring as a transmitter"

#cp libs/app/config.22h libs/app/config.h.bkp 

sed -i 's,//#define USE_MRF24_TX,#define USE_MRF24_TX,g' libs/app/config.h
sed -i 's,#define USE_MRF24_RX,//#define USE_MRF24_RX,g' libs/app/config.h

echo "Configure as Transmitter Tx ..."