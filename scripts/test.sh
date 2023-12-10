#!/bin/bash

# Function to generate a random data file
generate_random_file() {
    size=$1
    output_file=$2

    # Generate random data using /dev/urandom
    dd if=/dev/urandom of="$output_file" bs=1M count=$size status=progress

    echo "Generated $size MiB file: $output_file"
}

log_file="log.txt"

# Loop 5 times
for iteration in {1..3}; do
    echo "Iteration $iteration"

    sizes=(1 2 4 8 16 32 64 128 256 512 1024)

    for size in "${sizes[@]}"; do
        output_file="${size}_random_MiB.bin"
        generate_random_file $size $output_file
        echo "Encoding file: $output_file"
        ../build/app/encode "$output_file" >> "$log_file" 2>&1
        echo "Shuffling lines..."
        ./line_shuffler.sh $output_file.encode >> "$log_file" 2>&1
        echo "Decoding file: $output_file"
        ../build/app/decode "$output_file".encode >> "$log_file" 2>&1
        echo "----------------------" >> "$log_file"
        sudo rm "$output_file"*
    done
done

