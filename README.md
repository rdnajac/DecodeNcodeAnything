# DecodeNcodeAnything
###  Utilizing synthetic DNA 🧬 as a data storage 💾 medium
A refactoring of the [Adaptive DNA Storage Codec (ADS Codex)](https://github.com/lanl/adscodex) in "modern" C++.

## Table of Contents
- [External Dependencies](#external-dependencies)
- [Installation](#installation)
- [Running the Program](#running-the-program)
- [Features](#features)
- [Documentation](#documentation)
- [Plans for Future Releases](#plans-for-future-releases)
- [Acknowledgments](#acknowledgments)

## External Dependencies

- **zlib for C++:** a library for handling .gz files, necessary to support direct reading of compressedFASTQs. More information about zlib [here](https://www.zlib.net/).

- **CMake for Building:** You can download CMake from [cmake.org](https://cmake.org/) or with your package manager.

- **Doxygen for Documentation:** used for automatically generating documentation from source code comments. More information about Doxygen can be found [here](https://www.doxygen.nl/).
> Note that the Doxygen documentation for this project adheres to the style guide available [here](https://micro-os-plus.github.io/develop/doxygen-style-guide/).

## Installation

1. **Clone the DecodeNcodeAnything repository:**

    ```bash
    git clone https://github.com/rdnajac/DecodeNcodeAnything.git
    cd DecodeNcodeAnything
    ```

2. **Create a build directory:**

    ```bash
    mkdir build
    cd build
    ```

3. **Configure the project with CMake:**

    On Debian-based systems:

    ```
    cmake ..
    ```

    On Windows (Make sure your MinGW-x64 version is newer than 11.0.0):

    ```bash
    cmake -G "MinGW Makefiles" ..
    ```

4. **Build the project:**

    ```
    make
    ```

Adjust the build commands based on your specific build system or requirements. Alternatively, you can run the default installation script: `scripts/build.sh` from the repo's top-level directory.

## Documentation

To generate documentation using Doxygen, follow these steps:

1. **Install Doxygen:**

    Ensure that Doxygen is installed on your system. If it's not installed, you can typically install it using your package manager. 

    For example, on Debian-based systems:

    ```bash
    sudo apt-get install doxygen
    ```
    On Windows system:

    ```
    git clone https://github.com/doxygen/doxygen.git
    ```

    Refer to the [Doxygen installation guide](https://www.doxygen.nl/manual/install.html) for more details.

2. **Navigate to the project root:**

    ```bash
    cd /path/to/DecodeNcodeAnything
    ```

3. **Run the documentation generation script:**

    On Debian-based systems

    ```bash
    scripts/gen_docs.sh
    ```

    This script generates a new Doxyfile, configures it, and runs Doxygen to generate documentation in the `./docs` folder.

    On Windows system:

    Just use Visual Studio to construct the project. Then it will generate a doxygen.exe.

4. **Access the documentation:**

    Open the generated documentation by navigating to the specified output directory:

    ```bash
    cd /path/to/DecodeNcodeAnything/docs
    ```

    Open the `index.html` file in a web browser to explore the generated documentation.

## Running the Program
After cloning and building the the program, the executables (including test programs) are located in the build directory. Running the program is simple: 
```
./build/app/encode <file-to-be-encoded>
```
or alternatively, 
```
./build/app/encode <file-to-be-decoded>
```
The decoder expects the FASTQ files while the encoder can handle any readable file. 


## Features
###  Reed–Solomon Error Correction
Library written in C++ for module export.

> Reed–Solomon Error Correction is a mathematical technique that allows the correction of errors in transmitted or stored data to enhance reliability and robustness. It is widely used in various applications, including data storage, QR codes, and digital communication.

Resources for understanding Reed–Solomon error correction:
  - [Reed–Solomon Codes on Wikipedia](https://en.wikipedia.org/wiki/Reed%E2%80%93Solomon_error_correction)
  - [Introduction to Reed–Solomon Codes](https://www.cs.cmu.edu/~guyb/realworld/reedsolomon/reed_solomon_codes.html)

## TODO: Implement [Google's testing framework](https://github.com/google/googletest).

Example:

```cpp
#include <gtest/gtest.h>

TEST(ADSCodexTest, EncodingTest) {
    // Test encoding functionality of the ADS Codex
    // ...
    ASSERT_TRUE(/* Some condition indicating success */);
}

TEST(ADSCodexTest, DecodingTest) {
    // Test decoding functionality of the ADS Codex
    // ...
    ASSERT_TRUE(/* Some condition indicating success */);
}

// Add more tests as needed...

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

## Plans for Future Releases

1. **Performance Optimization with Lookup Tables:**
    - Introduce and leverage lookup tables for performance optimization. Lookup tables can enhance the efficiency of certain operations, contributing to faster encoding and decoding processes.
2. **Abstract Interface for Oligo Viability Criteria (H4G2):**
    - Introduce an abstract interface for evaluating the viability of oligonucleotides based on specific criteria. One such criteria, denoted as H4G2, prevents the inclusion of oligos with homopolymers longer than 4 nucleotides (for A, T, and C) or 2 nucleotides for G. Developers can extend this interface to implement custom viability criteria.
3. **Expanded Documentation:**
    - Enhance and expand the documentation to provide comprehensive guidance on usage and potential extensions.

These plans are subject to change based on community feedback and project priorities. Stay tuned for updates and announcements related to future releases.

If you have specific features or improvements you would like to see in future releases, feel free to contribute to the discussion on our [GitHub repository](https://github.com/rdnajac/DecodeNcodeAnything) or open a new issue.

## Acknowledgments

- [Adaptive DNA Storage Codec (ADS Codex)](https://github.com/lanl/adscodex): The foundation for this project.
- [Google Test](https://github.com/google/googletest): For providing a robust testing framework.
- [CMake](https://cmake.org/): Used for building the project.
- [Doxygen](https://www.doxygen.nl/): Used for generating documentation from source code comments.
- [Illumina](https://www.illumina.com/): For contributions to DNA sequencing technology.
- [Oxford Nanopore Technology](https://nanoporetech.com/): For advancements in nanopore sequencing.
- [Kilobaser](https://kilobaser.com/): For innovations in DNA synthesis technology.
- [Bjarne Stroustrup](http://www.stroustrup.com/): For his foundational contributions to C++ and for his guidance in this project.
