##
## This file is part of tinyPTC, UA version 2019
## Copyright (C) 2000-2002 Alessandro Gatti <a.gatti@tiscali.it>
## Copyright (C) 2002      Fred Howell <foohoo@shaw.ca>
## Copyright (C) 2000-2001 Glenn Fiedler <gaffer@gaffer.org>
## Copyright (C) 2019 by Francisco J. Gallego-Durán (@FranGallegoBR)
## 
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
##
###########################################
### MACROS
###########################################

# $(1) : Compiler
# $(2) : Object file to generate
# $(3) : Source file
# $(4) : Additional dependencies
# $(5) : Compiler flags
define COMPILE
$(2) : $(3) $(4)
	$(1) -c -o $(2) $(3) $(5)

endef

# $(1) : Source file
define C2O
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1))))
endef

# $(1) : Source file
define C2H
$(patsubst %.c,%.h,$(patsubst %.cpp,%.hpp,$(1)))
endef

###########################################
### CONFIG
###########################################
UNAME   := $(shell uname)
APP     := lib_spi.a
CCFLAGS := -Wall -pedantic
CFLAGS  := $(CCFLAGS)
CC 	  := g++
C       := gcc
AR      := ar
ARFLAGS := -crs
RANLIB  := ranlib
MKDIR   := mkdir -p
SRC     := src
OBJ     := obj

ifeq ($(UNAME),Darwin)
	OS := macOS
ifeq ($(UNAME),Linux)
	OS := linux
else ifeq ($(UNAME),Windows)
	OS := windows
endif

#ifdef APPLE
#	CC := clang
#	C := gcc
#endif

ifdef CROSSWIN
	C := x86_64-w64-mingw32-gcc
	CC := x86_64-w64-mingw32-g++
	AR := x86_64-w64-mingw32-ar
	RANLIB := x86_64-w64-mingw32-ranlib
	OS := windows
endif

ifdef DEBUG
   CCFLAGS += -g 
	CFLAGS  += -g
else ifdef SANITIZE
	CCFLAGS += -fsanitize=address -fno-omit-frame-pointer -O1 -g
	CFLAGS  += -fsanitize=address -fno-omit-frame-pointer -O1 -g
else 
   CCFLAGS += -O3
   CFLAGS  += -O3
endif

ALLCPPS    := $(shell find src/$(OS)/ -type f  -iname *.cpp)
ALLCS      := $(shell find src/$(OS)/ -type f  -iname *.c)
ALLOBJ     := $(foreach F,$(ALLCPPS) $(ALLCS),$(call C2O,$(F)))
SUBDIRS    := $(shell find $(SRC) -type d)
OBJSUBDIRS := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

.PHONY: info

# Generate Library
$(APP) : $(OBJSUBDIRS) $(ALLOBJ)
	$(AR) $(ARFLAGS) $(APP) $(ALLOBJ)
	$(RANLIB) $(APP)

# Generate rules for all objects
$(foreach F,$(ALLCPPS),$(eval $(call COMPILE,$(CC),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(CCFLAGS))))
$(foreach F,$(ALLCS),$(eval $(call COMPILE,$(C),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(CFLAGS))))

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCS))
	$(info $(ALLCSOBJ))

$(OBJSUBDIRS) : 
	$(MKDIR) $(OBJSUBDIRS)


## CLEAN rules
clean:
	$(RM) -r "./$(OBJ)"

cleanall: clean
	$(RM) "./$(APP)"
	
