#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <filesystem>
#include "io.hpp"
#include "oligo.cpp"

/**
 * @brief Codec class for handling files and Oligo data.
 */
class Codec {
private:
    std::vector<Oligo> oligo_vec; ///< Vector to store Oligo objects.
    std::string filename; ///< Name of the file.
    std::streampos filesize; ///< Size of the file.
    std::ifstream file; ///< Input file stream.

public:
    /**
     * @brief Default constructor.
     */
    Codec() : filesize(0) {}

    /**
     * @brief Constructor with filename.
     * @param filename The name of the file to process.
     */
    explicit Codec(const std::string& filename) : filename(filename), filesize(0), file(open_file(filename.c_str(), filesize)) {
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return; // Exit the constructor if the file couldn't be opened
        }
    }

    /**
     * @brief Destructor.
     * Closes the file if it's open.
     */
    ~Codec() {
        if (file.is_open())
        {
            file.close();
            std::cout << "File closed in the destructor." << std::endl;
        }
    }

    /**
     * @brief Function to get file size.
     * @return The size of the file.
     */
    std::streampos get_filesize() const { return filesize; }

    /**
     * @brief Function to get filename.
     * @return The name of the file.
     */
    std::string get_filename() const { return filename; }

    /**
     * @brief Function to get filetype/extension.
     * @return The filetype/extension of the file.
     */
    std::string get_filetype() const { return std::filesystem::path(get_filename()).extension().string(); }

    /**
     * @brief Function to print filename, filesize, and filetype.
     */
    void print_info() const {
        std::cout << "Filename: " << get_filename() << std::endl;
        std::cout << "Filesize: " << get_filesize() << " bytes" << std::endl;
        std::cout << "Filetype: " << get_filetype() << std::endl;
    }

    /**
     * @brief Function to print the vector of Oligo objects.
     */
    void print_oligos() const {
        for (const auto& oligo : oligo_vec)
            std::cout << oligo.seq() << std::endl;
    }

    /**
     * @brief Function to dump Oligo information to the console.
     */
    void oligodump() const {
        for (size_t i = 0; i < oligo_vec.size(); ++i) {
            // Print index
            std::cout << std::setw(8) << std::setfill('0') << i << " | ";

            // Print sequence
            std::cout << oligo_vec[i].seq() << " | ";

            // Get the uint64_t value
            uint64_t uint64Value = oligo_vec[i].data();

            // Print original characters (if printable)
            const uint8_t* byteArray = reinterpret_cast<const uint8_t*>(&uint64Value);
            for (size_t j = 0; j < sizeof(uint64_t); ++j) {
                char c = static_cast<char>(byteArray[j]);
                std::cout << (std::isprint(c) ? c : '.');
            }

            std::cout << std::endl;
        }
    }
/**
     * @brief Function to encode the file data into Oligo objects.
     */
    void encode()
    {
        size_t num_blocks = filesize / sizeof(uint64_t);
        size_t remaining_bytes = filesize % 64;
        oligo_vec.reserve(num_blocks + (remaining_bytes > 0 ? 1 : 0));

        // Read data into a uint64_t, construct Oligo, and emplace into the vector
        for (size_t i = 0; i < num_blocks; ++i) {
            uint64_t data_block;
            if (!file.read(reinterpret_cast<char*>(&data_block), sizeof(uint64_t))) {
                std::cerr << "Error reading file: " << filename << std::endl;
                oligo_vec.clear(); // Clear the vector in case of an error
                return; // Exit the constructor if there was an error reading the file
            }
            oligo_vec.emplace_back(64, data_block);
        }

        // TODO: Handle the remaining bytes using a single buffer
        // if (remaining_bytes > 0) {
        //     uint64_t data_block;
        //     if (!file.read(reinterpret_cast<char*>(&data_block), remaining_bytes)) {
        //         std::cerr << "Error reading remaining bytes from file: " << filename << std::endl;
        //         oligo_vec.clear(); // Clear the vector in case of an error
        //         return; // Exit the constructor if there was an error reading the remaining bytes
        //     }
        //     oligo_vec.emplace_back(remaining_bytes, data_block);
        // }
    }
    // Uncomment the following lines when Criteria class is finished
    /*
    // Function to get criteria
    Criteria get_criteria() const;

    // Function to set criteria
    void set_criteria(const Criteria& new_criteria);
    */
};

// Implementation of other member functions

