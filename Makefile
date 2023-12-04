CC = clang++
CXXFLAGS = -std=c++17 -Ilibs -Isrc
LIBRARIES = -pthread -lmysqlcppconn -lqrencode -lpng -lbcm2835 -lrt 
LIBRARIES += -Wall -pedantic
#LIBRARIES += -lSSD1306_OLED_RPI  
SRC_DIR = src
LIB_DIR = libs

LIBRARY_DIR := libs
OBJ_DIR = obj
BIN_DIR = bin

# Buscar todas las carpetas dentro de LIB_DIR
LIB_DIRS = $(wildcard $(LIB_DIR)/*)

# Generar nombres de bibliotecas y objetos dinámicamente
LIBS = $(foreach libdir,$(LIB_DIRS),$(LIBRARY_DIR)/$(notdir $(libdir))/lib$(notdir $(libdir)).a)
OBJS = $(foreach libdir,$(LIB_DIRS),$(addprefix $(OBJ_DIR)/$(notdir $(libdir))/, $(notdir $(wildcard $(libdir)/src/*.cpp:.cpp=.o))))

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

# Reglas para construir bibliotecas
$(LIBRARY_DIR)/%/lib%.a: $(OBJ_DIR)/%.o | $(LIBRARY_DIR)/%
	ar rcs $@ $<

# Reglas de compilación para los archivos fuente en SRC_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) 
	$(CC) $(CXXFLAGS) -c $< -o $@ -MMD -MP

# Reglas de compilación para los archivos fuente en cada carpeta de LIB_DIR
define compile_template
$(OBJ_DIR)/%.o: $(LIB_DIR)/$(1)/src/%.cpp | $(OBJ_DIR)/$(1)
	$(CC) $(CXXFLAGS) -c $$< -o $$@ -MMD -MP
endef

# Construir reglas de compilación para archivos en carpetas específicas de LIB_DIR
$(foreach libdir,$(LIB_DIRS),$(eval $(call compile_template,$(notdir $(libdir)))))

# Crear directorios si no existen
$(BIN_DIR) $(OBJ_DIR) $(LIBRARY_DIR) $(foreach libdir,$(LIB_DIRS),$(OBJ_DIR)/$(notdir $(libdir))) $(foreach libdir,$(LIB_DIRS),$(LIBRARY_DIR)/$(notdir $(libdir))):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIBRARY_DIR)
	rm -f log/*

run: $(APP)
	sudo $<

-include $(OBJS:.o=.d)
