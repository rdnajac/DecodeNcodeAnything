#include "mygzip.hpp"

int main() {
    // Test string
    std::string testString = "Hello, this is a test string for gzip compression and decompression.";

    // Compress
    Gzipper compressor("test_string.txt");
    std::ofstream testFile("test_string.txt");
    testFile << testString;
    testFile.close();

    if (compressor.process()) {
        std::cout << "Compression successful." << std::endl;
    } else {
        std::cerr << "Compression failed. Check the error messages above for details." << std::endl;
        return 1;
    }

    // Decompress
    Gzipper decompressor("test_string.txt.gz");
    if (decompressor.process()) {
        std::cout << "Decompression successful." << std::endl;
    } else {
        std::cerr << "Decompression failed. Check the error messages above for details." << std::endl;
        return 1;
    }

    // Read and print the decompressed file content
    std::ifstream decompressedFile("test_string.txt");
    std::string decompressedString((std::istreambuf_iterator<char>(decompressedFile)),
                                   std::istreambuf_iterator<char>());
    std::cout << "Decompressed content: " << decompressedString << std::endl;

    return 0;
}

