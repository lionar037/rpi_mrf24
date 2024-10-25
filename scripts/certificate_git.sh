#!/bin/bash

MAIL=8897224+lionar037@users.noreply.github.com

ssh-keygen -t ed25519 -C "$MAIL"


echo " "
cat ~/.ssh/id_ed25519.pub
echo "  "
echo "  copiar clave publica"





ssh -T git@github.com

git config --global user.email "$MAIL"
git config --global user.name "lionar037"


