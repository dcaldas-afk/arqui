#!/bin/bash

for hex in 1_*.hex; do
    base="${hex%.hex}"

    echo "Testando $base..."

    ./poxim "$hex" teste.out

    if diff -q "$base.out" teste.out > /dev/null; then
        echo "  OK"
    else
        echo "  FALHOU"
        diff -u "$base.out" teste.out | head -80
        exit 1
    fi
done

rm -f teste.out
echo "Todos passaram."
