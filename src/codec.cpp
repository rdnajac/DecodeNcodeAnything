#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <filesystem>
#include <ostream>
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
    std::vector<std::pair<Oligo, Oligo*>> oligo_duplex; // Oligo* points to an entry in oligo_vec
    std::vector<std::pair<Oligo, Oligo>> decode_duplex;
    std::vector<Oligo*> decode_vec; ///< Vector to store Oligo objects.

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
     * @brief Destructor. Closes the file if it's open.
     */
    ~Codec() {
        if (file.is_open())
            file.close();
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

            std::cout << std::setw(8) << std::setfill('0') << i << " | ";

            std::cout << oligo_vec[i].seq() << " | ";

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
        size_t remaining_bytes = filesize % 8;
        oligo_vec.reserve(num_blocks + (remaining_bytes ? 1 : 0));
        oligo_duplex.reserve(num_blocks + (remaining_bytes ? 1 : 0));

        // Read data into a uint64_t, construct Oligo, and emplace into the vector
        for (size_t i = 0; i < num_blocks; ++i) {
            uint64_t data_block;
            if (!file.read(reinterpret_cast<char*>(&data_block), sizeof(uint64_t))) {
                std::cerr << "Error reading file: " << filename << std::endl;
                oligo_vec.clear(); // Clear the vector in case of an error
                return; // Exit the constructor if there was an error reading the file
            }
            oligo_vec.emplace_back(MAX_BP, data_block);
            oligo_duplex.emplace_back(Oligo(MAX_BP, i), &oligo_vec.back());
        }

        // Handle the remaining bytes using a single buffer
        if (remaining_bytes > 0) {
            uint64_t data_block = 0;;
            if (!file.read(reinterpret_cast<char*>(&data_block), remaining_bytes)) {
                std::cerr << "Error reading remaining bytes from file: " << filename << std::endl;
                oligo_vec.clear(); // Clear the vector in case of an error
                oligo_duplex.clear();
                return; // Exit the constructor if there was an error reading the remaining bytes
            }
            oligo_vec.emplace_back(remaining_bytes * 8, data_block);  // Adjust the bit count
            oligo_duplex.emplace_back(Oligo(MAX_BP, num_blocks), &oligo_vec.back());
        }
    }

    /**
     * @brief Function to dump Oligo information from duplex to the console.
     */
    void dump_duplex() const {
        for (size_t i = 0; i < oligo_duplex.size(); ++i) {
            std::cout << std::setw(8) << std::setfill('0') << i << " | ";
            std::cout << oligo_duplex[i].first.seq() << "-" << oligo_duplex[i].second->seq() << std::endl;
        }
    }
    /**
     * @brief Function to convert and return oligo_duplex as a vetor of strings
     */
    std::vector<std::string> get_duplex_vec() {
        std::vector<std::string> nt_vec;
        nt_vec.reserve(oligo_duplex.size());

        for (const auto& [first, second] : oligo_duplex)
            nt_vec.emplace_back(first.seq() + second->seq());

        return nt_vec;
    }

    /**
     * @brief Function to dump Oligo information from duplex to a file.
     */
    void write_duplex() const {
        std::ofstream outfile(get_filename() + ".encode");

        if (!outfile.is_open()) {
            std::cerr << "Error opening file for writing: " << get_filename() + ".encode" << std::endl;
            return;
        }

        for (size_t i = 0; i < oligo_duplex.size(); ++i)
            outfile << oligo_duplex[i].first.seq() << oligo_duplex[i].second->seq() << std::endl;

        std::cout << "Input file encoded and written to: " << get_filename() + ".encode" << std::endl;
    }

    /**
     * @brief Function to encode the file data into Oligo objects.
     */
    void decode() {

        oligo_vec.clear();
        oligo_duplex.clear();
        decode_duplex.clear();
        size_t line_number = 0;

        for (std::string line; std::getline(file, line); line_number++) {
            if ((get_filetype() == ".fastq") && (line_number % 4 != 1))
                continue;
            if  (line.size() == 64) {
                Oligo index_oligo(line.substr(0, MAX_BP));
                Oligo data_oligo(line.substr(32, MAX_BP));
                decode_duplex.emplace_back(index_oligo, data_oligo);
            }
        }

        std::sort(decode_duplex.begin(), decode_duplex.end(), [](const auto& a, const auto& b) {
                return a.first.data() < b.first.data();
                });

        std::ofstream output_file(get_filename() + ".decode", std::ios::binary);
        if (!output_file.is_open()) {
            std::cerr << "Error opening output file: " << get_filename() + ".decode" << std::endl;
            oligo_vec.clear();
            decode_duplex.clear();
            return;
        }
        for (auto &o : decode_duplex)
            o.second.write_bin(output_file);

        output_file.close();
        std::cout << "Input file decoded and written to: " << get_filename() + ".decode" << std::endl;
    }

    //Uncomment the following lines when Criteria class is finished
    //Criteria get_criteria() const;
    //void set_criteria(const Criteria& new_criteria);
};

