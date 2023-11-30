CC = clang++
CXXFLAGS = -std=c++17 -Ilibs -Isrc
LIBRARIES = -pthread -lmysqlcppconn
SRC_DIR = src
LIB_DIR = libs
OBJ_DIR = obj
BIN_DIR = bin
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
LIB_DIRS = $(wildcard $(LIB_DIR)/*)
LIB_SRCS = $(foreach dir,$(LIB_DIRS),$(wildcard $(dir)/*.cpp))
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o) $(notdir $(LIB_SRCS:.cpp=.o))))

# Nombre predeterminado para el binario
DEFAULT_TARGET = mrf24_rpi

# Obtener el nombre del objetivo desde la línea de comandos
TARGET ?= $(DEFAULT_TARGET)

# Cambiar el nombre del binario según el objetivo
ifeq ($(TARGET), tx)
    APP = $(BIN_DIR)/mrf24_rpi_tx
else ifeq ($(TARGET), rx)
    APP = $(BIN_DIR)/mrf24_rpi_rx
else
    APP = $(BIN_DIR)/$(DEFAULT_TARGET)
endif

.PHONY: all clean

all: $(APP)

$(APP): $(OBJS) | $(BIN_DIR)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIBRARIES)

# Regla de compilación para los archivos de código fuente en SRC_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) 
	$(CC) $(CXXFLAGS) -c $< -o $@ -MMD -MP

# Regla de compilación para los archivos de código fuente en LIB_DIR y sus subdirectorios
define compile_template
$(info Compiling files in folder: $(LIB_DIR)/$(1))
$(OBJ_DIR)/%.o: $(LIB_DIR)/$(1)/%.cpp | $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c $$< -o $$@ -MMD -MP
endef

# Construir reglas de compilación para archivos en carpetas específicas de LIB_DIR
$(foreach libdir,$(LIB_DIRS),$(eval $(call compile_template,$(notdir $(libdir)))))

# Crear directorios si no existen
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Incluir las dependencias generadas automáticamente
-include $(OBJS:.o=.d)
