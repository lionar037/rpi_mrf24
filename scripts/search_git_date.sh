#!/bin/bash

#colocar la fecha del git commit a restaurar
GIT_COMMIT=$1

#YYYY-MM-DD
SINCE="2024-09-01"
#YYYY-MM-DD
UNTIL="2023-09-01"

if [[ $GIT_COMMIT == "date" ]]; then 
    git log --since=$SINCE --until=$UNTIL
fi 

if [[ -z $GIT_COMMIT ]]; then 
    echo "insertar el dato del comentario Git"
    exit 1
else
    git checkout $GIT_COMMIT

    git reset --hard $GIT_COMMIT
fi
