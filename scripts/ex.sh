#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <input_filename>"
  exit 1
fi

input_filename=$1
encoded_filename="${input_filename}.encode"
randomized_filename="${encoded_filename}.random"
fastq_filename="${randomized_filename}.fastq"
compressed_filename="${fastq_filename}.gz"

# Step 1: Build and run the test_encode-decode program
./build/tests/test_encode-decode

# Step 2: Run the randomizer utility
./utils/randomizer "$encoded_filename"

# Step 3: Run the gen_fastq utility
./utils/gen_fastq "$randomized_filename"

# Step 4: Prompt to compress the generated FASTQ file
read -p "Do you want to compress the FASTQ file? (y/n): " compress_choice
if [ "$compress_choice" == "y" ]; then
  gzip "$fastq_filename"
  echo "FASTQ file compressed. Compressed file: $compressed_filename"
else
  echo "Process completed. Uncompressed FASTQ file: $fastq_filename"
fi

