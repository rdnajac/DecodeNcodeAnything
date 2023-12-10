#!/bin/bash

# Generate a new Doxyfile in the ./docs directory
doxygen -g ./Doxyfile

# Set the desired configuration options in the Doxyfile
sed -i 's/^PROJECT_NAME.*$/PROJECT_NAME = "DecodeNcodeAnything"/' ./Doxyfile
sed -i 's/^RECURSIVE.*$/RECURSIVE = YES/' ./Doxyfile
sed -i 's/^EXTRACT_ALL.*$/EXTRACT_ALL = YES/' ./Doxyfile
sed -i 's/^EXTRACT_STATIC.*$/EXTRACT_STATIC = YES/' ./Doxyfile
sed -i 's/^EXTRACT_PRIVATE.*$/EXTRACT_PRIVATE = YES/' ./Doxyfile
sed -i 's/^OUTPUT_DIRECTORY.*$/OUTPUT_DIRECTORY = .\/docs/' ./Doxyfile
sed -i 's#^USE_MDFILE_AS_MAINPAGE.*$#USE_MDFILE_AS_MAINPAGE = docs/design.md#' ./Doxyfile

# Run Doxygen with the generated configuration
doxygen ./Doxyfile
echo "Doxyfile created and configured. Doxygen documentation generated in ./docs"

