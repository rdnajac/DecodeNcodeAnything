// codec.hpp
#ifndef CODEC_HPP
#define CODEC_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "io.hpp"
#include "oligo.hpp"
// #include "criteria.hpp"

/**
 * @brief Codec class for handling files and Oligo data.
 */
class Codec {
private:
    std::vector<Oligo> oligoVector;
    std::string filename;
    std::streampos filesize;
    std::ifstream file; // Include the ifstream as a member to close it in the destructor
    // Criteria criteria;

public:
    // Default constructor
    Codec();

    /**
     * @brief Constructor with filename.
     * @param filename The name of the file to process.
     */
    explicit Codec(const std::string& filename);

    // Destructor
    ~Codec();

    /**
     * @brief Function to get file size.
     * @return The size of the file.
     */
    std::streampos get_file_size() const;

    /**
     * @brief Function to get filename.
     * @return The name of the file.
     */
    std::string get_filename() const;

    /**
     * @brief Function to print filename, filesize, and filetype/extension.
     */
    void print_info() const;

    /**
     * @brief Function to print the vector of Oligo objects.
     */
    void print_oligos() const;

    /**
     * @brief Function to print the vector of Oligo objects with original characters using a ternary operator.
     */
    void oligodump() const;

    // Uncomment the following lines when Criteria class is finished
    /*
    // Function to get criteria
    Criteria get_criteria() const;

    // Function to set criteria
    void set_criteria(const Criteria& new_criteria);
    */
};

#endif // CODEC_HPP

