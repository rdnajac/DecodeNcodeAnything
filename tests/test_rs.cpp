#include <iostream>
#include "../src/oligo.cpp"

int main() {
    // Example usage of the Oligo class
    Oligo oligo1(10, 0b1101010101010101);  // Example oligo with 10 base pairs and data
    Oligo oligo2(8, 0b1111000011110000);   // Another example oligo with 8 base pairs and data

    // Test the encode function
    std::cout << "Encoding oligo1:" << std::endl;
    //oligo1.encode();

    // Test the decode function
    std::cout << "\nDecoding oligo2:" << std::endl;
    //oligo2.decode();

    return 0;
}

