#!/bin/bash

#colocar la fecha del git commit a restaurar
GIT_COMMIT=$1

#YYYY-MM-DD
SINCE="2024-10-25"
#YYYY-MM-DD
UNTIL="2023-01-01"

if [[ $GIT_COMMIT == "date" ]]; then 
    git log --since=$SINCE --until=$UNTIL
#    exit 1
fi

if [[ -z $GIT_COMMIT ]]; then 
    echo "insertar el dato del comentario Git"
    exit 1
else
    git checkout $GIT_COMMIT

 #   git reset --hard $GIT_COMMIT
fi
