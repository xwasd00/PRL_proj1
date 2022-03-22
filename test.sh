#!/bin/bash
PROJ=oems
SRC=*.cpp
NUMBERS=8
PROCESSES=20

# preklad cpp zdrojaku
mpic++ -o $PROJ $SRC

# vyrobeni souboru s random cisly
dd if=/dev/random bs=1 count=$NUMBERS of=numbers 2>/dev/null

# spusteni
mpirun -np $PROCESSES ./$PROJ

# uklid
rm -f $PROJ numbers

