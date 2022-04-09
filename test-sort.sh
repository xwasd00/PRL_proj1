#!/bin/bash
PROJ=oems
SRC=$PROJ.cpp
HDR=$PROJ.h
NUMBERS=8
PROCESSES=19
CYCLES=100


# preklad cpp zdrojaku
mpic++ -o $PROJ $SRC $HDR
I=0
while (( I < CYCLES )); do
    # vyrobeni souboru s random cisly
    dd if=/dev/random bs=1 count=$NUMBERS of=numbers 2>/dev/null

    # spusteni
    #mpirun -oversubscribe -np $PROCESSES ./$PROJ
    OUT=$(mpirun -np $PROCESSES ./$PROJ)

    # kontrola
    UNSORTED=$(cat numbers | od -t u1 -An | sed 's/\s\+/ /g')
    UNSORTED=${UNSORTED:1}
    SORTED=$(printf "$UNSORTED" | sed 's/\s\+/\n/g' | sort -n)
    PARAGON=$(printf "$UNSORTED \n$SORTED")

    if [[ "$OUT" == "$PARAGON" ]]; then
        echo "cycle $I: OK"
    else
        echo "cycle $I: fail"
        echo "program:"
        echo "$OUT"
        echo "bash script"
        echo "$PARAGON"
        exit
    fi

    ((I++))
done

# uklid
rm -f $PROJ numbers