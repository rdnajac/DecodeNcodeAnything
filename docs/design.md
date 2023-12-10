# Designing DNA Storage in C++

## Decode Ncode Anything
*Caroline Leighton, Xinjin (Synthia) Li, Ryan Najac*
*COMS 4995 Design using C++*
*Fall 2023*

### Introduction

#### Problems Addressed
The exponential growth of the global datasphere necessitates a paradigm shift in storage solutions. Traditional methods like HDDs and SSDs are inadequate for the anticipated surge from 100 to 175 Zettabytes by 2025. This project aims to design an efficient and durable DNA storage system to meet the evolving demands of data preservation.

### Project Overview

#### Final Design Overview
Our approach involves opening, reading binary data, constructing sequences, and decoding. Key components include the `Oligos` class, `Codec` class, a vector for efficient data manipulation, and the `ReedSolomon` template class for error correction.

The proposed DNA storage design addresses current limitations, offering a robust, efficient, and reliable solution for the impending surge in global data. By combining innovative algorithms, well-designed classes, compatibility considerations, and strategic tooling choices, our project aims to contribute to the evolution of data storage technology.

#### Advantages of Our Algorithm
- **Efficient Storage and Retrieval:** Leveraging the `Oligos` class ensures faster bitwise operations.
- **Error Correction and Data Reliability:** Integration of Reed-Solomon enhances data reliability.
- **Flexibility and Compatibility:** Our design utilizes standard C++ libraries, ensuring compatibility.

#### Tooling Choices for Efficient Development
- **CMake:** Used for building the project, providing a platform-independent and efficient way to manage dependencies and compile the project seamlessly.
- **Doxygen:** Used for generating documentation from source code comments, promoting clarity and understanding for developers.
- **zlib:** Important for handling .gz compressed files, providing efficient and reliable compression and decompression algorithms, essential for working with large genomic datasets.

### Project Structure

#### app
Source code files for the main encoding and decoding applications.

#### build
Generated with `cmake`
- **app:** Directory where compiled executable files for the 'app' directory are stored.
- **CMakeCache.txt:** CMake cache file storing configuration settings.
- **CMakeFiles:** Directory containing CMake-generated files.
- **cmake_install.cmake:** CMake installation file.
- **Makefile:** Makefile for building the project.
- **src:** Directory where compiled object files for the 'src' directory are stored.
- **tests:** Directory where compiled object files for the 'tests' directory are stored.

#### include: DNA Sequence Utilities
- **io.hpp:** Header file containing declarations for input/output functions.
- **utils.hpp:** Header file containing declarations for utility functions.

##### Nucleotide Conversion
##### Sequence Analysis
##### Sequence Matching and Alignment
##### Sequence Prefix and Suffix Matching
##### Sequence Difference Calculation

Leverages modern C++20 features: Unordered Map, Optionals, Range-Based Loops, Algorithm Functions, contributing to improved readability, conciseness, and maintainability.

#### scripts
An assortment of shell scripts to aid in building and testing the project
---
##### Oligonucleotide Class

###### Oligos Class: Efficient Storage and Retrieval
Efficiently stores DNA sequences using an unsigned 64-bit data block, minimizing overhead by performing operations, such as slicing and conversion to a string representation, only upon request.

*C++ Class Representation:*
Utilizes a C++ class aligning with efficiency and encapsulation principles. The use of an unsigned 64-bit data block allows for bitwise operations, enhancing speed and reducing memory footprint.

###### Class Overview
Defines an `Oligo` class to represent oligonucleotides (DNA sequences) with constructors, comparison operators, and various member functions for oligo manipulation.

###### Class Members
- `size_t basepairs`: Number of base pairs in the oligonucleotide (limited to `MAX_BP`).
- `uint64_t data_block`: Data block storing the binary representation of the oligonucleotide.
###### File I/O
- `write_bin(std::ofstream &of)`: Write binary data to a file.

Modern C++ Features include Constructors Initialization List, Range-Based For Loop, std::optional, std::min, Operator Overloading, Conditional Compilation (`#ifdef`), TODO for Spaceship Operator (<=>).

---

##### Codec Class
###### Codec Class: Managing File Stream and Operations
Crucial in managing file-related operations, including opening, closing, and obtaining the filesize. Encapsulates the complexities of interacting with the file stream, providing a streamlined interface for the rest of the system.

*C++ Class Representation:*
A C++ class for the Codec concept offers a modular and organized approach to file handling. The class encapsulates file-related functionalities, promoting code readability and maintainability. The ability to easily open, close, and obtain file size enhances the overall robustness of the system.

###### Class Overview
Defines a `Codec` class for handling files and Oligo data, including file I/O operations, encoding and decoding functions, and vector manipulation for Oligo objects.

###### Class Members
- `std::vector<Oligo> oligo_vec`: Vector to store Oligo objects.
- `std::string filename`: Name of the file.
- `std::streampos filesize`: Size of the file.
- `std::vector<Oligo*> decode_vec`: Vector to store Oligo objects for decoding.

The `Codec` class exhibits the use of modern C++ features, enhancing code readability, expressiveness, and efficiency.
- Initialization List, Smart Pointers, Range-Based For Loop, Lambda Expressions, Filesystem Library, Sort Algorithm, Binary File Output, Type Aliases (auto).

---

#### utils: Modern C++ Utility Libraries Retooled
- **Seamless Integration:** Utills seamlessly integrates with existing C++ code, enhancing organization and maintainability while eliminating the need for external processes. This promotes overall application cohesiveness.

- **Dynamic Linking:** Utills supports dynamic linking, enabling updates without modifying the source code. This adaptability is particularly beneficial for evolving compression standards or library improvements.

- **Consistent Interface:** With a uniform interface, Utills encourages a standardized approach to file compression and decompression within the codebase. This consistency enhances code readability and facilitates collaboration among developers.

- **Tailored Error Handling:** The C++ Reed-Solomon library allows developers to implement specific error-handling strategies, ensuring robust and resilient data processing.

- **Enhanced Control:** Combining Reed-Solomon error correction with compression provides advanced control over data integrity. Developers can manage error correction parameters and strategies to meet reliability requirements consistently.

- **Performance Optimization** Updated and improved with modern C++

##### Efficient Handling of Compressed Files
Ufficiently manages file processing and compression by leveraging the zlib library. It can distinguish between regular and gzipped files, providing versatility in handling different file types.

##### Reed-Solomon Error Correction Implementation with C++ Classes and Concepts
Seamlessly incorporates Reed-Solomon error correction using the `ReedSolomon` template class, representing the Reed-Solomon error correction code.
- **Galois Field:** Incorporates a `GaloisField` attribute, fundamental to Reed-Solomon operations.

