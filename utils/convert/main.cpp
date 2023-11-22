#include <iostream>
#include <string>
#include <unordered_map>

#include "convert.hpp"

int main() {
    std::string choice, sequence;
    std::cout << "Welcome to the DNA Sequence Converter!" << std::endl;

    while (true) {
        std::cout << "Enter '1' to convert text to DNA, '2' to convert DNA to text, or 'Q' to quit: ";
        std::cin >> choice;

        if (choice == "Q") {
            break;
        } else if (choice != "1" && choice != "2") {
            std::cout << "Invalid choice. Please enter '1' or '2' or 'Q'." << std::endl;
            continue;
        }

        std::cout << "Enter the sequence you want to convert: ";
        std::cin.ignore(); // ignore any leftover characters in the input stream
        std::getline(std::cin, sequence);

        if (choice == "1") {
            std::string dnaSequence = textToDNA(sequence);
            std::cout << "The DNA sequence is: " << dnaSequence << std::endl;
        } else {
            if (!isValidDNA(sequence)) {
                std::cout << "Invalid DNA sequence. Please ensure it contains only 'A', 'G', 'T', 'C' and is a multiple of 4 in length." << std::endl;
                continue;
            }
            std::string textSequence = dnaToText(sequence);
            std::cout << "The text sequence is: " << textSequence << std::endl;
        }
    }

    std::cout << "Thank you for using the DNA Sequence Converter!" << std::endl;
    return 0;
}
