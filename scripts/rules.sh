#!/bin/bash

LINK_RULES=$PWD/scripts/rules.gdb
LINK_APP=$PWD/bin/mrf24_app

echo "$PWD"

sudo gdb -x $LINK_RULES $LINK_APP
