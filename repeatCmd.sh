#!/bin/bash

for ((i=1; i<=100; i++)); do
    echo "Iteración $i:"
    uname -m
    echo "----------------"
done
