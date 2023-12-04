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
#$(1)   Source file
#$(1)   src/tinyPTC/xbcde.cpp
#$(1)   obj/tinyPTC/xbcde.o
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
APP         := app
CFLAGS     := -Wall -pedantic
CCFLAGS     	:= $(CFLAGS) -std=c++17
CC          := g++
C			:= gcc
MKDIR       := mkdir -p
SRC         := src
OBJ         := obj
LIBDIR := libs

#LIBS	:= $(LIBDIR)/spi/lib_spi.a
#LIBS += $(LIBDIR)/oled/lib_oled.a
#
#LIBS += $(LIBDIR)/others/lib_others.a
#LIBS += $(LIBDIR)/files/lib_files.a
#LIBS += $(LIBDIR)/app/lib_app.a
#LIBS += $(LIBDIR)/qr/lib_qr.a
#LIBS += $(LIBDIR)/mrf24/lib_mrf24.a


LIBS := $(CFLAGS)
INCDIRS := -I$(SRC) -I$(LIBDIR)
#LIBS		+= -lX11 -lXext

LIBS += LIBRARIES = -pthread -lmysqlcppconn -lqrencode -lpng -lbcm2835 -lrt 
LIBS += -lSSD1306_OLED_RPI 


#para el uso commando es make DEBUG=1
ifdef DEBUG 
	CCFLAGS += -g
else
	CCFLAGS += -O3
endif


ALLCPPS 	:= $(shell find src/ -type f -iname *.cpp)
#ALLOCPPSOBJ  	:= $(patsubst %.cpp,%.o,$(ALLCPPS))
ALLCS		:= $(shell find src/ -type f -iname *.c)
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



#%.o : %.c
#	$(C) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CFLAGS)

#%.o : %.cpp
#	$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS)

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))

$(OBJSUBDIRS):
	$(MKDIR) $(OBJSUBDIRS) 

clean:
	$(RM) -r "./$(OBJ)"

cleanall: clean
	$(RM) "./$(APP)"
libs:
	$(MAKE)	-C $(LIBDIR)
libs-clean:
	$(MAKE) -C $(LIBDIR) clean
libs-cleanall:
	$(MAKE) -C $(LIBDIR) cleanall

