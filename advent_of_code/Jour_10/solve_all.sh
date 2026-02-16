#!/bin/bash

INPUT="input.txt"    # fichier contenant tes lignes
OUTPUT="results.txt"

> "$OUTPUT"  # vide le fichier de résultats

lineno=0
while IFS= read -r line || [[ -n "$line" ]]; do
    ((lineno++))
    
    # extraire coefficients
    coeffs=$(echo "$line" | grep -oP '\{[^}]+\}' | tr -d '{}' | tr ',' ' ')
    if [[ -z "$coeffs" ]]; then
        echo "Line $lineno: no coefficients found" >> "$OUTPUT"
        continue
    fi
    
    coeff_array=($coeffs)
    n_coeff=${#coeff_array[@]}
    
    # extraire tuples
    tuples=$(echo "$line" | grep -oP '\([^)]+\)')
    invalid=false
    
    for t in $tuples; do
        indices=$(echo "$t" | tr -d '()' | tr ',' ' ')
        for i in $indices; do
            if (( i >= n_coeff )); then
                invalid=true
                break 2
            fi
        done
    done
    
    if $invalid; then
        echo "Line $lineno: invalid indices" >> "$OUTPUT"
        continue
    fi
    
    # créer le LP temporaire
    TMP="tmp.lp"
    echo "Maximize" > $TMP
    echo " obj: $(echo "${coeff_array[@]}" | sed 's/ / + /g')" >> $TMP
    echo "Subject To" >> $TMP
    constraint_idx=1
    for t in $tuples; do
        indices=$(echo "$t" | tr -d '()' | tr ',' ' ')
        expr=""
        for i in $indices; do
            expr+="x$i + "
        done
        expr=${expr%+ }  # enlever le dernier +
        echo " c$constraint_idx: $expr <= 1" >> $TMP
        ((constraint_idx++))
    done
    echo "Bounds" >> $TMP
    for ((i=0;i<n_coeff;i++)); do
        echo " 0 <= x$i <= 1" >> $TMP
    done
    echo "Binary" >> $TMP
    for ((i=0;i<n_coeff;i++)); do
        echo " x$i" >> $TMP
    done
    echo "End" >> $TMP
    
    # résoudre avec GLPK
    glpsol --lp $TMP -o tmp.sol > /dev/null 2>&1
    if [[ ! -f tmp.sol ]]; then
        echo "Line $lineno: GLPK failed" >> "$OUTPUT"
        continue
    fi
    
    # extraire résultat
    value=$(grep -oP 'Objective:\s+\K[\d.]+' tmp.sol)
    echo "Line $lineno: $value" >> "$OUTPUT"
    
    rm -f tmp.sol $TMP
done < "$INPUT"

