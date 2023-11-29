// (c) 2010-2012 nerdfever.com
// Address for IEEE 802.15.4 radio

#pragma once

#define MY_LONG_ADDRESS 	(0x0123456789abcdefull)		// device MAC address (8 bytes, 64 bit)
#define MY_SHORT_ADDRESS	(0xaa55)					// short (2 byte) 802.15.4 address
#define MY_PAN_ID			(0x0f00)					// PAN identifier
/*
#define PACKET_BUFFERS 0x01
#define RX_BUFFER_SIZE 0x01
#define WRITE_SOFTRST 0x00
#define READ_SOFTRST 0x00
*/
bool RADIO_CS ;
//bool RFIE;
bool RADIO_TX ;
bool RADIO_RX;
bool RADIO_CLK;

#define SPI_INTERRUPTS

