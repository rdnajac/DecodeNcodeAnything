// codec.cpp
#include "codec.hpp"

// Default constructor
Codec::Codec() : filesize(0) {}

// Constructor with filename
Codec::Codec(const std::string& filename) : filename(filename), filesize(0), file(open_file(filename.c_str(), filesize))
{
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return; // Exit the constructor if the file couldn't be opened
    }
    size_t numBlocks = filesize / sizeof(uint64_t);
    size_t remainingBytes = filesize % 64;
    oligoVector.reserve(numBlocks + (remainingBytes > 0 ? 1 : 0));

    // Read data into a uint64_t, construct Oligo, and emplace into the vector
    for (size_t i = 0; i < numBlocks; ++i) {
        uint64_t data_block;
        if (!file.read(reinterpret_cast<char*>(&data_block), sizeof(uint64_t))) {
            std::cerr << "Error reading file: " << filename << std::endl;
            oligoVector.clear(); // Clear the vector in case of an error
            return; // Exit the constructor if there was an error reading the file
        }
        oligoVector.emplace_back(64, data_block);
    }
    /* TODO:  Handle the remaining bytes using a single buffer
    if (remainingBytes > 0) {
        uint64_t data_block;

        // Read the remaining bytes from the file
        if (!file.read(reinterpret_cast<char*>(&data_block), remainingBytes)) {
            std::cerr << "Error reading remaining bytes from file: " << filename << std::endl;
            oligoVector.clear(); // Clear the vector in case of an error
            return; // Exit the constructor if there was an error reading the remaining bytes
        }
        oligoVector.emplace_back(remainingBytes, data_block);
    }
    */
}

// Destructor
Codec::~Codec()
{
    if (file.is_open())
    {
        file.close();
        std::cout << "File closed in the destructor." << std::endl;
    }
}

// Function to get file size
std::streampos Codec::get_file_size() const
{
    return filesize;
}

// Function to get filename
std::string Codec::get_filename() const
{
    return filename;
}

// Function to print filename, filesize, and filetype/extension
void Codec::print_info() const
{
    std::cout << "Filename: " << filename << std::endl;
    std::cout << "Filesize: " << filesize << " bytes" << std::endl;
    std::cout << "Filetype/Extension: " << std::filesystem::path(filename).extension() << std::endl;
}

// Function to print the vector of Oligo objects
void Codec::print_oligos() const
{
    for (const auto& oligo : oligoVector)
        std::cout << oligo.seq() << std::endl;
}

void Codec::oligodump() const
{
    for (size_t i = 0; i < oligoVector.size(); ++i) {
        // Print index
        std::cout << std::setw(8) << std::setfill('0') << i << " | ";

        // Print sequence
        std::cout << oligoVector[i].seq() << " | ";

        // Get the uint64_t value
        uint64_t uint64Value = oligoVector[i].uint64();

        // Print original characters (if printable)
        const uint8_t* byteArray = reinterpret_cast<const uint8_t*>(&uint64Value);
        for (size_t j = 0; j < sizeof(uint64_t); ++j) {
            char c = static_cast<char>(byteArray[j]);
            std::cout << (std::isprint(c) ? c : '.');
        }

        std::cout << std::endl;
    }
}

// Uncomment the following lines when Criteria class is finished
/*
// Function to get criteria
Criteria Codec::get_criteria() const
{
return criteria;
}

// Function to set criteria
void Codec::set_criteria(const Criteria& new_criteria)
{
criteria = new_criteria;
}
*/

