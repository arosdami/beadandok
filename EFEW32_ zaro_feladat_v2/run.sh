#!/bin/bash
gcc main.c -o telefonkonyv -lncurses
if [ $? -ne 0 ]; then
    echo "Fordítási hiba történt!"
    exit 1
fi
echo "Fordítás sikeres, a program most indul..."
./telefonkonyv