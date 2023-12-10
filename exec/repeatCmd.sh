#!/bin/bash

for ((i=1; i<=100; i++)); do
    echo "Iteración $i:"
    make run
    echo "----------------"
done
