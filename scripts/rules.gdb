#gdb ./bin/mrf24_tx_app
gdb ./bin/mrf24_rx_app

# Establecer puntos de interrupción

break  MRF24J40::Mrf24j::init
break  MRF24J40::Mrf24j::write_short

break  MRF24J40::Mrf24j::rx_disable
break  MRF24J40::Mrf24j::interrupts
break  MRF24J40::Mrf24j::digitalWrite

break OLED::Oled_t::start
break OLED::Oled_t::test
break OLED::Oled_t::function


# Ejecutar el programa
run

# Verificar si el archivo se abrió correctamente
#print filePath
#print file

# Continuar la ejecución hasta el próximo breakpoint
continue

# Inspeccionar las variables al decodificar la imagen PNG
print lsb_tmp
print address

# Ver el contenido del buffer PNG cargado
#x/10xb in_png_binary.data()

# Finalizar la depuración
quit