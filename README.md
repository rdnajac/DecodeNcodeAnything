# DecodeNcodeAnything
###  Utilizing synthetic DNA 🧬 as a digital storage 💾 medium
A refactoring of the [Adaptive DNA Storage Codec (ADS Codex)](https://github.com/lanl/adscodex) in "modern" C++.

## Table of Contents
- [External Dependencies](#external-dependencies)
- [Installation](#installation)
- [Documentation](#documentation)
- [TODO](#todo)
  - [Implement C++20 Features](#implement-c20-features)
  - [Implement Reed–Solomon Error Correction](#implement-reedsolomon-error-correction)
  - [Implementing Google's Testing Framework](#implement-googles-testing-framework)
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

    ```bash
    cmake ..
    ```

4. **Build the project:**

    ```bash
    make
    ```

Adjust the build commands based on your specific build system or requirements. Alternatively, you can run the default installation script: `scripts/build.sh` from the repo's top-level directory.

## Documentation

To generate documentation using Doxygen, follow these steps:

1. **Install Doxygen:**

    Ensure that Doxygen is installed on your system. If it's not installed, you can typically install it using your package manager. For example, on Debian-based systems:

    ```bash
    sudo apt-get install doxygen
    ```

    On Red Hat-based systems:

    ```bash
    sudo yum install doxygen
    ```

    Refer to the [Doxygen installation guide](https://www.doxygen.nl/manual/install.html) for more details.

2. **Navigate to the project root:**

    ```bash
    cd /path/to/DecodeNcodeAnything
    ```

3. **Run the documentation generation script:**

    ```bash
    scripts/gen_docs.sh
    ```

    This script generates a new Doxyfile, configures it, and runs Doxygen to generate documentation in the `./docs` folder.

4. **Access the documentation:**

    Open the generated documentation by navigating to the specified output directory:

    ```bash
    cd /path/to/DecodeNcodeAnything/docs
    ```

    Open the `index.html` file in a web browser to explore the generated documentation.

## TODO

### Implement C++20 Features

1. **Concepts:**
   Express constraints on template parameters, improving template code readability.
     ```cpp
     template <typename T> concept Integral = std::is_integral_v<T>;
     void function(Integral auto value) { /* Implementation */ }
     ```

2. **Ranges:**
   Library for range-based algorithms and views, simplifying the manipulation of sequences of values.
     ```cpp
     std::vector<int> numbers = {1, 2, 3, 4, 5};
     auto evenNumbers = numbers | std::views::filter([](int n) { return n % 2 == 0; });
     ```

3. **Coroutines:**
   More efficient and readable asynchronous code.
     ```cpp
     #include <iostream>
     #include <coroutine>
     generator<int> generateNumbers(int start, int end) {
         for (int i = start; i <= end; ++i)
             co_yield i;
     }
     // Usage:
     auto numbers = generateNumbers(1, 5);
     for (int num : numbers) {
         std::cout << num << " ";
     }
     ```

4. **Three-Way Comparison Operator (`<=>`):**
   The spaceship operator simplifies the implementation of comparison operations.
     ```cpp
     struct Person {
         std::string name;
         int age;
         auto operator<=>(const Person& other) const = default;
     };
     ```

5. **Calendar and Timezone Library:**
   Enhanced `<chrono>` library for working with calendars and time zones.
     ```cpp
     #include <chrono>
     #include <iostream>
     auto currentTime = std::chrono::system_clock::now();
     auto timePoint = std::chrono::floor<std::chrono::days>(currentTime);
     ```

6. **Improvements to `std::span`:**
   Class template for non-owning references to contiguous sequences.
     ```cpp
     #include <span>
     #include <iostream>
     std::array<int, 5> data = {1, 2, 3, 4, 5};
     std::span<int, 5> dataSpan = data;
     ```

7. **Modules:**
   Improve the modularity of C++ code and reduce compilation times.
     ```cpp
     import std.core; // hypothetical module import syntax
     int main() {
         std::cout << "Hello, Modules!\n";
     }
     ```

8. **Improved `constexpr` Support:**
   Allows for more complex computations at compile-time.
     ```cpp
     constexpr int fibonacci(int n) {
         return (n <= 1) ? n : (fibonacci(n - 1) + fibonacci(n - 2));
     }
     constexpr int result = fibonacci(5);
     ```


9. **Changes to `std::string`:**
    - **`starts_with` and `ends_with` Member Functions:** Check whether a string starts or ends with a specified substring.
       ```cpp
       std::string myString = "Hello, World!";
       if (myString.starts_with("Hello")) {
           // Code to handle the case where the string starts with "Hello"
       }
       ```
       - **`reserve` Function for Capacity Management:** Preallocate memory for a specified number of characters, helping to manage the capacity of the string more efficiently.
       ```cpp
       std::string myString;
       myString.reserve(420); // Reserve space for 420 characters
       ```
       - **`erase` for Removing Elements:** A more flexible idiom for removing elements from a string at specific positions or ranges.
       ```cpp
       std::string myString = "Hello, World!";
       myString.erase(myString.begin() + 7); // Erase the character at position 7
       ```

### Implement Reed–Solomon Error Correction

Existing C++ libraries for Reed–Solomon error correction to consider:
  - [schifra](https://github.com/ArashPartow/schifra): A C++ template library for Reed–Solomon error correction.
  - [ReedSolomon](https://github.com/FluffyJay1/ReedSolomon): A C++ implementation of Reed–Solomon codes.
  - [librs](https://www.kernel.org/doc/html/v4.18/core-api/librs.html): The Linux kernel's Reed–Solomon library.

> Reed–Solomon Error Correction is a mathematical technique that allows the correction of errors in transmitted or stored data to enhance reliability and robustness. It is widely used in various applications, including data storage, QR codes, and digital communication.

Resources for understanding Reed–Solomon error correction:
  - [Reed–Solomon Codes on Wikipedia](https://en.wikipedia.org/wiki/Reed%E2%80%93Solomon_error_correction)
  - [Introduction to Reed–Solomon Codes](https://www.cs.cmu.edu/~guyb/realworld/reedsolomon/reed_solomon_codes.html)

### Implement [Google's testing framework](https://github.com/google/googletest).

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

