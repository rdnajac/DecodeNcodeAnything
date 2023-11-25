#ifndef IO_HPP
#define IO_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

// Check if zlib is linked
#ifdef ZLIB_FOUND
#include <zlib.h>
#endif

/**
 * @brief Opens a file and gets its size.
 * @param filename The name of the file to open.
 * @param file_size The size of the file (output parameter).
 * @return An input file stream.
 */
std::ifstream open_file(const char* filename, std::streampos& file_size);

/**
 * @brief Processes the content of a regular file.
 * @param buffer The buffer containing the file content.
 */
void process_regular_file(const std::vector<char>& buffer);

/**
 * @brief Processes the content of a gzipped file.
 * @param buffer The buffer containing the gzipped file content.
 */
void process_gzipped_file(const std::vector<char>& buffer);

/**
 * @brief Processes a file by opening it, reading its content, and handling .gz files.
 * @param filename The name of the file to process.
 */
void process_file(const char* filename);

#endif // IO_HPP

