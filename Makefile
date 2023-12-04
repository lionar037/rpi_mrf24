CC = clang++
CXXFLAGS = -std=c++17 -Ilibs -Isrc
LIBRARIES = -pthread -lmysqlcppconn -lqrencode -lpng -lbcm2835 -lrt 
LIBRARIES += -Wall -pedantic
SRC_DIR = src
LIB_DIR = libs

LIBRARY_DIR := libs
OBJ_DIR = obj
BIN_DIR = bin

# Buscar carpetas con archivos fuente en src/ de los subdirectorios de LIB_DIR
LIB_DIRS := $(wildcard $(LIB_DIR)/*)
SRC_DIRS := $(addsuffix /src, $(LIB_DIRS))
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

DEFAULT_TARGET = mrf24_rpi

TARGET ?= $(DEFAULT_TARGET)

ifeq ($(TARGET), tx)
    APP = $(BIN_DIR)/mrf24_rpi_tx
else ifeq ($(TARGET), rx)
    APP = $(BIN_DIR)/mrf24_rpi_rx
else
    APP = $(BIN_DIR)/$(DEFAULT_TARGET)
endif

.PHONY: all clean

all: $(APP) $(LIBS)

# Construir reglas de compilación para archivos en carpetas específicas de LIB_DIR
$(foreach dir,$(LIB_DIRS),$(eval $(call compile_template,$(dir))))

# Reglas de compilación para los archivos fuente en SRC_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) 
	$(CC) $(CXXFLAGS) -c $< -o $@ -MMD -MP

define compile_template
$(info Compiling files in folder: $(1)/src)
$(OBJ_DIR)/%.o: $(1)/src/%.cpp | $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c $$< -o $$@ -MMD -MP
endef

# Crear directorios si no existen
$(BIN_DIR) $(OBJ_DIR) $(LIBRARY_DIR) $(foreach dir,$(LIB_DIRS),$(OBJ_DIR)/$(notdir $(dir))) $(foreach dir,$(LIB_DIRS),$(LIBRARY_DIR)/$(notdir $(dir))):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIBRARY_DIR)
	rm -f log/*

run: $(APP)
	sudo $<

-include $(OBJS:.o=.d)
