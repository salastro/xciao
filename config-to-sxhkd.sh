#!/usr/bin/env sh

CONFIG="config.h"
FILE="sxhkd"

cp $CONFIG $FILE

vim -n -u NONE -s macro1.vim $FILE
sed -i 'p' $FILE
vim -n -u NONE -s macro2.vim $FILE
