/**
 * @file
 * @brief Functions for DNA representation of characters and file processing.
 */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

/**
 * @brief Converts a character to its DNA representation.
 * @param ch The input character.
 * @return The DNA representation of the input character.
 */
std::string char2dna(char ch) {
    std::string binary = "";
    for (int i = 7; i >= 0; --i) binary += ((ch & (1 << i)) ? "1" : "0");

    std::string dna = "";
    for (size_t i = 0; i < binary.length(); i += 2)
        if (binary.substr(i, 2) == "00") dna += "A";
        else if (binary.substr(i, 2) == "10") dna += "G";
        else if (binary.substr(i, 2) == "01") dna += "T";
        else if (binary.substr(i, 2) == "11") dna += "C";
    return dna;
}

/**
 * @brief Prints line numbers in hex format.
 * @param i The current position in the data.
 */
void print_line_numbers(size_t i) {
    std::cout << std::setw(8) << std::setfill('0') << std::hex << i << ": ";
}

/**
 * @brief Prints DNA representations of characters.
 * @param buffer The data buffer.
 * @param i The current position in the data.
 * @param size The size of the data buffer.
 */
void print_oligos(const unsigned char *buffer, size_t i, size_t size) {
    for (size_t j = 0; j < 16; ++j)
        if (i + j < size) std::cout << char2dna(buffer[i + j]) << ((j % 8 == 7) ? " " : "");
        else std::cout << ' ';
}

/**
 * @brief Prints original characters or dots for non-printable characters.
 * @param buffer The data buffer.
 * @param i The current position in the data.
 * @param size The size of the data buffer.
 */
void print_chars(const unsigned char *buffer, size_t i, size_t size) {
    for (size_t j = 0; j < 16 && i + j < size; ++j)
        std::cout << (isprint(buffer[i + j]) ? buffer[i + j] : '.');
}

/**
 * @brief Dumps the DNA representation of the data along with line numbers.
 * 
 * @param data The input data.
 * @param size The size of the data.
 */
void oligo_dump(const void *data, size_t size) {
    const unsigned char *buffer = static_cast<const unsigned char *>(data);

    for (size_t i = 0; i < size; i += 16) {
        print_line_numbers(i);
        print_oligos(buffer, i, size);
        print_chars(buffer, i, size);
        std::cout << std::endl;
    }
}

/**
 * @brief Opens a file and gets its size.
 * @param filename The name of the file to open.
 * @param file_size The size of the file (output parameter).
 * @return An input file stream.
 */
std::ifstream open_file(const char *filename, std::streampos &file_size) {
    std::ifstream file(filename, std::ios::binary);

    if (file) file.seekg(0, std::ios::end), file_size = file.tellg(), file.seekg(0, std::ios::beg);
    return file;
}

/**
 * @brief Processes a file by opening it, reading its content, and dumping the DNA representation.
 * @param filename The name of the file to process.
 */
void process_file(const char *filename) {
    std::streampos file_size;
    std::ifstream file = open_file(filename, file_size);

    if (!file) return std::cerr << "Error opening file: " << filename << std::endl, void();

    std::vector<char> buffer(file_size);
    file.read(buffer.data(), file_size);

    oligo_dump(buffer.data(), buffer.size());
}

