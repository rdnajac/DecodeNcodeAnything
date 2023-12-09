#!/bin/bash

# Function to generate a random data file
generate_random_file() {
    size=$1
    output_file=$2

    # Generate random data using /dev/urandom
    dd if=/dev/urandom of="$output_file" bs=1G count=$size status=progress

    echo "Generated $size GiB file: $output_file"
}

log_file="log.txt"

# Loop 5 times
for iteration in {1..5}; do
    echo "Iteration $iteration"

    sizes=(1 2 4 8 16 32)

    for size in "${sizes[@]}"; do
        output_file="random_data_${size}GiB.bin"
        generate_random_file $size $output_file

        echo "Encoding file: $output_file" >> "$log_file"
        ../build/app/encode "$output_file" >> "$log_file" 2>&1
        echo "Decoding file: $output_file" >> "$log_file"
        //../build/app/decode "$output_file".encoded >> "$log_file" 2>&1
        echo "----------------------" >> "$log_file"

        sudo rm "$output_file"
    done
done

