
# Designing DNA Storage in C++
**Decode Ncode Anything**  
*Caroline Leighton, Xinjin (Synthia) Li, Ryan Najac*  
*COMS 4995 Design using C++*  
*Fall 2023*

### 1. Problems Addressed
The exponential growth of the global datasphere necessitates a paradigm shift in storage solutions. Traditional methods like HDDs and SSDs are inadequate for the anticipated surge from 100 to 175 Zettabytes by 2025. This project aims to design an efficient and durable DNA storage system to meet the evolving demands of data preservation.

### 2. Alternatives Considered
Current DNA storage technologies face challenges in short reads, long reads, and nanopore sequencing. Our design integrates Reed-Solomon error correction to enhance data reliability, addressing these challenges.

### 3. Final Design Overview
Our approach involves opening, reading binary data, constructing sequences, and decoding. Key components include the Oligos class, Codec class, a vector for efficient data manipulation, and the ReedSolomon template class for error correction.

#### Oligos Class: Efficient Storage and Retrieval
The Oligos class efficiently stores DNA sequences using an unsigned 64-bit data block. It minimizes overhead by performing operations, such as slicing and conversion to a string representation, only upon request. This design choice optimizes for performance and resource usage.

*C++ Class Representation:*
Utilizing a C++ class for Oligos aligns with the principles of efficiency and encapsulation. The use of an unsigned 64-bit data block allows for bitwise operations, enhancing speed and reducing memory footprint.

#### Codec Class: Managing File Stream and Operations
The Codec class plays a crucial role in managing file-related operations, including opening, closing, and obtaining the filesize. This class encapsulates the complexities of interacting with the file stream, providing a streamlined interface for the rest of the system. The default constructor initializes the Codec object, while the constructor with a filename allows for easy instantiation with a specific file. The destructor ensures the proper closing of the file, enhancing resource management.

*C++ Class Representation:*
Choosing a C++ class for the Codec concept offers a modular and organized approach to file handling. The class encapsulates file-related functionalities, promoting code readability and maintainability. The ability to easily open, close, and obtain file size through dedicated member functions enhances the overall robustness of the system. The class acts as a cohesive unit, abstracting away the intricacies of file stream management.

#### Reed-Solomon Error Correction: Ensuring Data Reliability
In data transmission and storage, error correction is vital for data integrity. Reed-Solomon error correction involves adding redundant bits during encoding for error detection and correction during decoding.

#### Reed-Solomon Implementation with C++ Classes
Our project seamlessly incorporates Reed-Solomon error correction using the `ReedSolomon` template class. This class represents the Reed-Solomon error correction code, providing functions for encoding, decoding, and error correction.

- **Galois Field:** The class incorporates a `GaloisField` attribute, fundamental to Reed-Solomon operations.

### 4. Advantages of Our Algorithm
- **Efficient Storage and Retrieval:** Leveraging the Oligos class ensures faster bitwise operations.
- **Error Correction and Data Reliability:** Integration of Reed-Solomon enhances data reliability.
- **Flexibility and Compatibility:** Our design utilizes standard C++ libraries, ensuring compatibility.

### 5. Tooling Choices for Efficient Development
- **CMake:** Used for building the project. CMake simplifies the build process, providing a platform-independent and efficient way to manage dependencies and compile the project seamlessly.

- **Doxygen:** Used for generating documentation from source code comments. Doxygen automates the documentation process, ensuring that code documentation remains up-to-date and accessible. This choice promotes clarity and understanding for developers interacting with the codebase.

-  **zlib:** Important for handling .gz compressed files, especially since FASTQ sequencing files are generally compressed due to their size. zlib provides efficient and reliable compression and decompression algorithms, essential for working with large genomic datasets.

### 6. Future Considerations
Asynchronous Reading/Writing, a C++20 feature, could enhance performance. Given the directional nature of real-world data flows, it wasn't a primary focus.

### 7. Conclusion
The proposed DNA storage design addresses current limitations, offering a robust, efficient, and reliable solution for the impending surge in global data. By combining innovative algorithms, well-designed classes, compatibility considerations, and strategic tooling choices, our project aims to contribute to the evolution of data storage technology.

---

