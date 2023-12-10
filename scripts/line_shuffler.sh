#!/bin/bash

# Check if a file is provided as an argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# Save the filename
filename=$1

# Check if the file exists
if [ ! -f "$filename" ]; then
    echo "File not found: $filename"
    exit 1
fi

# Create a temporary file to store the randomized lines
tempfile=$(mktemp)

# Randomize the lines using the shuf command
shuf "$filename" > "$tempfile"

# Overwrite the original file with the randomized lines
mv "$tempfile" "$filename"

echo "Lines randomized in $filename"

