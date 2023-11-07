#include <iostream>
#include <string>
#include <unordered_map>

// Function to convert a character to its DNA representation
std::string charToDNA(char ch) {
    std::string binary = "";
    for (int i = 7; i >= 0; --i) {
        binary += ((ch & (1 << i)) ? "1" : "0");
    }

    std::string dna = "";
    for (size_t i = 0; i < binary.length(); i += 2) {
        if (binary.substr(i, 2) == "00") {
            dna += "A";
        } else if (binary.substr(i, 2) == "10") {
            dna += "G";
        } else if (binary.substr(i, 2) == "01") {
            dna += "T";
        } else if (binary.substr(i, 2) == "11") {
            dna += "C";
        }
    }
    return dna;
}

// Function to convert DNA sequence back to character
char dnaToChar(const std::string &dna) {
    int ascii = 0;
    for (size_t i = 0; i < dna.length(); i++) {
        ascii <<= 2;
        switch (dna[i]) {
            case 'A': break;
            case 'G': ascii += 2; break;
            case 'T': ascii += 1; break;
            case 'C': ascii += 3; break;
        }
    }
    return static_cast<char>(ascii);
}

// Function to check if the DNA is valid
bool isValidDNA(const std::string &dna) {
    if (dna.length() % 4 != 0) return false;
    for (char nucleotide : dna) {
        if (nucleotide != 'A' && nucleotide != 'G' && nucleotide != 'T' && nucleotide != 'C') {
            return false;
        }
    }
    return true;
}

// Function to convert string to DNA sequence
std::string textToDNA(const std::string &text) {
    std::string dna = "";
    for (char ch : text) {
        dna += charToDNA(ch);
    }
    return dna;
}

// Function to convert DNA sequence to string
std::string dnaToText(const std::string &dna) {
    std::string text = "";
    for (size_t i = 0; i < dna.length(); i += 4) {
        text += dnaToChar(dna.substr(i, 4));
    }
    return text;
}

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
