CC = clang++
CXXFLAGS = -std=c++17 -Ilibs -Isrc
LIBRARIES = -pthread -lmysqlcppconn -lqrencode -lpng -lbcm2835 -lrt 
LIBRARIES += -Wall -pedantic
#LIBRARIES += -lSSD1306_OLED_RPI  
SRC_DIR = src
LIB_DIR = libs

LIBRARY_DIR := libs
LIBS := $(LIBRARY_DIR)/lib_spi.a
LIBS += $(LIBRARY_DIR)/oled/lib_oled.a
OBJ_DIR = obj
BIN_DIR = bin

# Buscar archivos fuente en SRC_DIR y en todas las carpetas src/ de los subdirectorios de LIB_DIR
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(foreach dir,$(LIB_DIR),$(wildcard $(dir)/src/*.cpp))
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

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

$(LIBRARY_DIR)/oled/lib_oled.a: $(OBJ_DIR)/oled.o | $(LIBRARY_DIR)/oled
	ar rcs $@ $<

#$(LIBRARY_DIR)/spi/lib_spi.a: $(OBJ_DIR)/spi.o | $(LIBRARY_DIR)/spi
#	ar rcs $@ $<

$(LIBRARY_DIR)/lib_spi.a: $(OBJ_DIR)/spi.o | $(LIBRARY_DIR)/
	ar rcs $@ $<

$(APP): $(OBJS) $(LIBS) | $(BIN_DIR)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIBRARIES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) 
	$(CC) $(CXXFLAGS) -c $< -o $@ -MMD -MP

define compile_template
$(info Compiling files in folder: $(1)/src)
$(OBJ_DIR)/%.o: $(1)/src/%.cpp | $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c $$< -o $$@ -MMD -MP
endef

# Ajuste para compilar archivos de src/ en la carpeta spi
$(OBJ_DIR)/%.o: $(LIB_DIR)/spi/src/%.cpp | $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c $< -o $@ -MMD -MP

# Ajuste para compilar archivos de src/ en la carpeta oled
$(OBJ_DIR)/%.o: $(LIB_DIR)/oled/src/%.cpp | $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c $< -o $@ -MMD -MP


# Construir reglas de compilación para archivos en carpetas específicas de LIB_DIR
$(foreach libdir,$(LIB_DIR),$(eval $(call compile_template,$(libdir))))

$(BIN_DIR) $(OBJ_DIR) $(LIBRARY_DIR)/oled $(LIBRARY_DIR)/spi:
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -f log/*
	rm -rf  $(LIBRARY_DIR)/*.a 

run: $(APP)
	sudo $<

-include $(OBJS:.o=.d)
