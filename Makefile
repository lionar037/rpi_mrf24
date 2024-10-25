#######################################APPLICATION##################################################
################################################################################################
########################################MACROS##################################################
################################################################################################
#$(1)
#$(2)   Object file to generate
#$(3)   Source file
#$(4)   Additional dependencies
#$(5)   Compiler flags
define COMPILE
$(2) : $(3) $(4)
	$(1) -c -o $(2) $(3) $(5)
endef

define C2O
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1))))
endef
#$(1) Source file
define C2H
$(patsubst %.c,%.h,$(patsubst %.cpp,%.hpp,$(1)))
endef
################################################################################################
################################################################################################
################################################################################################v
PROJECT_NAME 	:=	mrf24_prj
OLED 			:= 	oled

# Detectar si es de 32 o 64 bits
ARCH := $(shell uname -m)

ifeq ($(ARCH),aarch64)
    APP := bin/mrf24_tx_app
    $(info aarch64 mrf24_tx_app )
else ifeq ($(ARCH),armv7l) 
    $(info armv7l mrf24_rx_app)
    APP := bin/mrf24_rx_app
else ifeq ($(ARCH),x86_64)
    APP := bin/mrf24_app_rx
    $(info x86_64 detectado OS de 64 bits) 
else
    $(info ningun OS detectado)
	APP := bin/mrf24_app
endif

#APP         := bin/mrf24_app
CFLAGS     	:= -Wall -pedantic
CCFLAGS     := $(CFLAGS) -std=c++17
CC          := g++
C			:= gcc
MKDIR       := mkdir -p
SRC         := $(PROJECT_NAME)
OBJ         := obj
LIBDIR 		:= $(PROJECT_NAME)


# Compilar con clang++ si es de 64 bits
ifeq ($(ARCH),x86_64)
	CC := g++
	CC := g++
else
	CC := g++
endif


LIBS := $(CFLAGS)
INCDIRS := -I$(SRC) -I$(LIBDIR)
#LIBS		+= -lX11 -lXext

LIBS += -pthread -lmysqlcppconn -lqrencode -lpng -lz
LIBS += -lbcm2835 -lrt -lmosquitto -lcrypto


# Detectar la arquitectura del sistema


ifeq ($(ARCH),x86_64)
    # Si es de 64 bits (x86_64), utilizar la biblioteca SSD1306_OLED_RPI
    LIBS += -lSSD1306_OLED_RPI  
    $(info x86_64 detectado OS de 64 bits)
else ifeq ($(ARCH),aarch64)
    # Si es de 64 bits (ARM), utilizar la biblioteca SSD1306_OLED_RPI
    LIBS += -lSSD1306_OLED_RPI  
    $(info aarch64 detectado OS de 64 bits)
else ifeq ($(ARCH),armv7l) # O cualquier otra arquitectura de 32 bits que necesites verificar
    # Si es de 32 bits (ARM), no usar la biblioteca SSD1306_OLED_RPI
    $(info armv7l detectado OS de 32 bits)
    LIBS += -lSSD1306_OLED_RPI
    # No se agrega LIBS para 32 bits
else
    # Si es una arquitectura de 32 bits no específica
    $(info OS de 32 bits detectado)
endif


#para el uso commando es make DEBUG=1
ifdef DEBUG 
	CCFLAGS += -g
else
	CCFLAGS += -O3
endif


ALLCPPS 	:= $(shell find $(PROJECT_NAME)/ -type f -iname *.cpp)
#ALLOCPPSOBJ  	:= $(patsubst %.cpp,%.o,$(ALLCPPS))
ALLCS		:= $(shell find $(PROJECT_NAME)/ -type f -iname *.c)
#ALLCSOBJ	:= $(patsubst %.c,%.o,$(ALLCS))
SUBDIRS 	:= $(shell find $(SRC) -type d)
OBJSUBDIRS 	:= $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))
ALLOBJ 		:= $(foreach F,$(ALLCPPS) $(ALLCS),$(call C2O,$(F)))

.PHONY: info libs libs-clean libs-cleanall
#Generate APP
$(APP) : $(OBJSUBDIRS) $(ALLOBJ)
	$(CC) -o $(APP) $(ALLOBJ) $(LIBS)

#Generate rules for all objects

$(foreach F,$(ALLCPPS),$(eval $(call COMPILE,$(CC),$(call C2O,$(F)),$(F),$(call C2H$(F)),$(CCFLAGS) $(INCDIRS))))
$(foreach F,$(ALLCS),$(eval $(call COMPILE,$(C),$(call C2O,$(F)),$(F),$(call C2H$(F)),$(CFLAGS) $(INCDIRS))))

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))

$(OBJSUBDIRS):
	$(MKDIR) $(OBJSUBDIRS) 

run: $(APP)
	sudo $<

clean:
	$(RM) -r "./$(OBJ)"

cleanall: clean
	$(RM) "./$(APP)"

usr-libs:
	$(MAKE) -C $(OLED)/

oled:
	$(MAKE) -C $(OLED)/	

libs:
	$(MAKE)	-C $(LIBDIR)/
	$(MAKE) -C $(OLED)/

libs-clean:
	$(MAKE) clean -C $(LIBDIR)/ 
	$(MAKE) clean -C $(OLED)/

libs-cleanall:
	$(MAKE) cleanall -C $(LIBDIR)/ 

