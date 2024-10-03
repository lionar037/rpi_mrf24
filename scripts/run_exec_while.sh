#!/bin/bash

# Definir la función
PrinterFunction() {
    for ((i=1; i<=100; i++)); do
        make run
    done
}

# Llamar a la función
PrinterFunction