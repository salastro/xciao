#!/usr/bin/env sh

CONFIG="config.h"
FILE="sxhkd"

cp $CONFIG $FILE

sed -i '1,2d;$d' $FILE
sed -i '$d' $FILE
sed -i -r '/^\s*$/d' $FILE
vim -n -u NONE -s macro1.vim $FILE
sort -r -u -o $FILE $FILE
sed -i 'p' $FILE
echo >> $FILE
vim -n -u NONE -s macro2.vim $FILE

# cat $FILE
