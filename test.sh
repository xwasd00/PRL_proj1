#!/bin/bash
PROJ=oems
SRC=*.c

if [ $# -lt 1 ];then 
    numbers=16;
else
    numbers=$1;
fi;

#preklad cpp zdrojaku
mpic++ -o $PROJ $SRC

#vyrobeni souboru s random cisly
dd if=/dev/random bs=1 count=$numbers of=numbers 2>/dev/null

#spusteni
mpirun -np $numbers ./$PROJ

#uklid
rm -f $PROJ numbers

