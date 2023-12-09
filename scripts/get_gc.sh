#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

filename=$1

if [ ! -f "$filename" ]; then
    echo "Error: File '$filename' not found."
    exit 1
fi

# Count occurrences of each nucleotide
count_A=$(grep -o 'A' "$filename" | wc -l)
count_C=$(grep -o 'C' "$filename" | wc -l)
count_G=$(grep -o 'G' "$filename" | wc -l)
count_T=$(grep -o 'T' "$filename" | wc -l)

# Calculate GC content
total_nucleotides=$((count_A + count_C + count_G + count_T))
gc_content=$(( (count_G + count_C) * 100 / total_nucleotides))

# Display results
echo "Counts in $filename:"
echo "A: $count_A"
echo "C: $count_C"
echo "G: $count_G"
echo "T: $count_T"

echo "GC Content: $gc_content%"

