#!/bin/bash

# Verificar si se está ejecutando como root
if [ "$(id -u)" -ne 0 ]; then
  echo "Este script debe ser ejecutado como root. Usa sudo." >&2
  exit 1
fi

# Ruta del archivo de configuración de swap
SWAP_FILE="/etc/dphys-swapfile"

# Comprobar si el archivo de configuración existe
if [ ! -f "$SWAP_FILE" ]; then
  echo "El archivo de configuración $SWAP_FILE no existe. Abortando." >&2
  exit 1
fi

# Respaldar el archivo original antes de modificarlo
echo "Creando respaldo del archivo original en /etc/dphys-swapfile.bak"
cp "$SWAP_FILE" "$SWAP_FILE.bak"

# Modificar el valor de CONF_SWAPSIZE a 512
echo "Modificando el archivo de configuración para cambiar el tamaño de swap a 512MB"
sed -i 's/CONF_SWAPSIZE=.*/CONF_SWAPSIZE=512/' "$SWAP_FILE"

# Reiniciar el servicio de swap
echo "Reiniciando el servicio dphys-swapfile para aplicar los cambios..."
systemctl restart dphys-swapfile

# Verificar el nuevo tamaño del swap
echo "Verificando el nuevo tamaño del swap:"
free -h

echo "El tamaño del swap ha sido cambiado a 512MB con éxito."
