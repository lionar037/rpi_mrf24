################################################################################################
###################################LIBRARY_TINYPTC##############################################
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
#$(1)   src/oled/oled.cpp
#$(1)   obj/oled/oled.o
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
AR	:=	ar
ARFLAGS 	:= -crs
RANLIB		:= ranlib
APP         := liboled.a
CCFLAGS     := -Wall -pedantic
CCFLAGS		+= -std=c++17
CFLAGS     	:= $(CCFLAGS)
CC          := g++
C			:= gcc
MKDIR       := mkdir -p
SRC         := src
OBJ         := obj
#LIBS		:= -lX11 -lXext

#para el uso commando es make DEBUG=1
ifdef DEBUG 
	CFLAGS += -g
else
	CFLAGS += -O3
endif


ALLCPPS 	:= $(shell find src/ -type f -iname *.cpp)
ALLCS		:= $(shell find src/ -type f -iname *.c)
SUBDIRS := $(shell find $(SRC) -type d)
OBJSUBDIRS := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))
ALLOBJ :=       $(foreach F,$(ALLCPPS) $(ALLCS),$(call C2O,$(F)))

.PHONY: info clean cleanall
#Generate library
$(APP) : $(OBJSUBDIRS) $(ALLOBJ)
	$(AR) $(ARFLAGS) $(APP) $(ALLOBJ)
	$(RANLIB) $(APP)

#Generate rules for all objects
$(foreach F,$(ALLCPPS),$(eval $(call COMPILE,$(CC),$(call C2O,$(F)),$(F),$(call C2H$(F)),$(CCFLAGS))))
$(foreach F,$(ALLCS),$(eval $(call COMPILE,$(C),$(call C2O,$(F)),$(F),$(call C2H$(F)),$(CFLAGS))))

#%.o : %.c
#	$(C) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CFLAGS)

#%.o : %.cpp
#	$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS)

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))


$(OBJSUBDIRS):
	$(MKDIR) $(OBJSUBDIRS) 

dbg:
	$(info $(ALLCS))
	$(info $(ALLCSOBJ))

clean:
	$(RM) -r "./$(OBJ)"

cleanall: clean
	$(RM) "./$(APP)"
	

