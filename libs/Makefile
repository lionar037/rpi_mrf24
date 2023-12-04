.PHONY: all clean cleanall


all:
	$(MAKE) -C app
	$(MAKE) -C files
	$(MAKE) -C mrf24
	$(MAKE) -C oled
	$(MAKE) -C others
	$(MAKE) -C qr
	$(MAKE) -C spi

clear:
	$(MAKE) -C spi all
	$(MAKE) -C oled clean
	$(MAKE) -C app clean
	$(MAKE) -C files clean
	$(MAKE) -C mrf24 clean
	$(MAKE) -C others clean
	$(MAKE) -C qr clean
	
clearall:
	$(MAKE) -C spi cleanall
	$(MAKE) -C oled cleanall
	$(MAKE) -C app cleanall
	$(MAKE) -C files cleanall
	$(MAKE) -C mrf24 cleanall
	$(MAKE) -C others cleanall
	$(MAKE) -C qr cleanall
