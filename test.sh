#!/bin/bash
PROJ=oems
SRC=$PROJ.cpp
HDR=$PROJ.h
NUMBERS=8
PROCESSES=19

# preklad cpp zdrojaku
mpic++ -o $PROJ $SRC $HDR

# vyrobeni souboru s random cisly
dd if=/dev/random bs=1 count=$NUMBERS of=numbers 2>/dev/null

# spusteni
mpirun -oversubscribe -np $PROCESSES ./$PROJ
#mpirun -np $PROCESSES ./$PROJ

# uklid
rm -f $PROJ numbers

