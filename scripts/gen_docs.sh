#!/bin/bash

# Create the docs directory if it doesn't exist
mkdir -p ./docs

# Generate a new Doxyfile in the ./docs directory
doxygen -g ./docs/Doxyfile

# Set the desired configuration options in the Doxyfile
sed -i 's/^PROJECT_NAME.*$/PROJECT_NAME = "DecodeNcodeAnything"/' ./docs/Doxyfile
sed -i 's/^RECURSIVE.*$/RECURSIVE = YES/' ./docs/Doxyfile
sed -i 's/^EXTRACT_ALL.*$/EXTRACT_ALL = YES/' ./docs/Doxyfile
sed -i 's/^EXTRACT_STATIC.*$/EXTRACT_STATIC = YES/' ./docs/Doxyfile
sed -i 's/^EXTRACT_PRIVATE.*$/EXTRACT_PRIVATE = YES/' ./docs/Doxyfile
sed -i 's/^OUTPUT_DIRECTORY.*$/OUTPUT_DIRECTORY = .\/docs/' ./docs/Doxyfile

# Run Doxygen with the generated configuration
doxygen ./docs/Doxyfile

echo "Doxyfile generated, configured, and Doxygen documentation generated in ./docs"

