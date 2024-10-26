#!/bin/bash

# Verifica si el script se está ejecutando como root
if [ "$EUID" -ne 0 ]; then
  echo "Por favor, ejecuta el script como root o con sudo."
  exit 1
fi

# Tamaño deseado del archivo de swap en MB
SWAP_SIZE_MB=512
SWAP_FILE="/swapfile"

echo "Desactivando el archivo de swap actual..."
swapoff $SWAP_FILE

echo "Eliminando el archivo de swap actual..."
rm -f $SWAP_FILE

echo "Creando un nuevo archivo de swap de $SWAP_SIZE_MB MB..."
dd if=/dev/zero of=$SWAP_FILE bs=1M count=$SWAP_SIZE_MB

echo "Estableciendo permisos en el archivo de swap..."
chmod 600 $SWAP_FILE

echo "Formateando el archivo de swap..."
mkswap $SWAP_FILE

echo "Activando el nuevo archivo de swap..."
swapon $SWAP_FILE

# Verificar si está en /etc/fstab
if ! grep -q "$SWAP_FILE" /etc/fstab; then
  echo "Añadiendo el archivo de swap a /etc/fstab para activarlo al inicio..."
  echo "$SWAP_FILE none swap sw 0 0" >> /etc/fstab
else
  echo "El archivo de swap ya está configurado en /etc/fstab."
fi

echo "Verificación del espacio de swap:"
swapon --show
free -h

echo "Ampliación del espacio de swap a $SWAP_SIZE_MB MB completada."
