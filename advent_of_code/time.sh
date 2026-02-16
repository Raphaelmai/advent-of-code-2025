#!/bin/bash

make
total=$(make | grep "Temps" | awk -F' ' '{print $4}' | awk '{s+=$1} END{print s}')

echo "Temps total d'execution : $total"

