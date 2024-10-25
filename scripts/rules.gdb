gdb ./bin/mrf24_app

# Establecer puntos de interrupción
break OLED::Oled_t::start
break OLED::Oled_t::test
break OLED::Oled_t::function

break  MRF24J40::Mrf24j::rx_disable
break  MRF24J40::Mrf24j::interrupts
break  MRF24J40::Mrf24j::digitalWrite


# Ejecutar el programa
run

# Verificar si el archivo se abrió correctamente
print filePath
print file

# Continuar la ejecución hasta el próximo breakpoint
continue

# Inspeccionar las variables al decodificar la imagen PNG
print image_width
print image_height
print in_size

# Ver el contenido del buffer PNG cargado
x/10xb in_png_binary.data()

# Finalizar la depuración
quit