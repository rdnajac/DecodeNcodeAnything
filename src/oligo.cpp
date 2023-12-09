#include <string>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "utils.hpp"


/**
 * @brief Maximum number of base pairs allowed for an oligonucleotide.
 */
const size_t MAX_BP = 32;

/**
 * @brief Represents an oligonucleotide (DNA sequence).
 */
class Oligo {
private:
    /**
     * @brief Number of base pairs in the oligonucleotide (limited to MAX_BP).
     */
    size_t basepairs;

    /**
     * @brief Data block storing the binary representation of the oligonucleotide.
     */
    uint64_t data_block;

public:
    /**
     * @brief Default constructor.
     */
    Oligo() : basepairs(0), data_block(0) {}

    /**
     * @brief Parameterized constructor.
     * @param bp The number of base pairs in the oligonucleotide (limited to MAX_BP).
     * @param val The value of the oligonucleotide.
     */
    Oligo(size_t bp, uint64_t val) : basepairs(std::min(bp, MAX_BP)), data_block(val) {}

    /**
     * @brief Constructor from string.
     * @param s The string representation of the oligonucleotide.
     */
    Oligo(const std::string& s) : basepairs(s.length() > MAX_BP ? 0 : s.length()), data_block(0) {
        for (char c : s) {
            std::optional<int> nt = char2nt(c);
            if (nt.has_value()) {
                data_block = (data_block << 2) | nt.value();
            }
            else {
                // Handle case where char2nt(c) returned std::nullopt
                // This could involve skipping the bitwise operation or setting a default value.
                // Example: data_block = (data_block << 2) | 0; // Setting data_block to 0 if conversion fails
            }
        }
    }

    /**
     * @brief Get the number of base pairs in the oligonucleotide.
     * @return The number of base pairs.
     */
    size_t bp() const { return basepairs; }

    /**
     * @brief Get the data block of the oligonucleotide.
     * @return The data block.
     */
    uint64_t data() const { return data_block; }

    /**
     * @brief Get the string representation of the data_block.
     * @return The string representation of the data_block.
     */
    std::string seq() const {
        std::string result;
        result.reserve(basepairs);

        for (size_t i = 0; i < std::min(basepairs, MAX_BP); ++i) {
            int nt = static_cast<int>((data_block >> (2 * (basepairs - i - 1))) & 0x3);
            result += nt2string(nt);
        }
        return result;
    }

    /**
     * @brief Compare the oligo with another oligo.
     * @param other The other oligo to compare.
     * @return -1 if the oligo is less than the other, 0 if equal, and 1 if greater.
     */
    int cmp(const Oligo& other) const {
        // Compare lengths of sequences
        if (bp() != other.bp())
            return (bp() < other.bp()) ? -1 : 1;

        // Compare sequences character by character
        for (size_t i = 0; i < bp(); ++i) {
            int nt1 = (*this)[i];
            int nt2 = other[i];

            if (nt1 != nt2)
                return (nt1 < nt2) ? -1 : 1;
        }
        return 0;  // Sequences are identical
    }

    bool operator==(const Oligo& other) const { return cmp(other) == 0; }
    bool operator!=(const Oligo& other) const { return cmp(other) != 0; }
    bool operator<(const Oligo& other) const { return cmp(other) < 0; }
    bool operator<=(const Oligo& other) const { return cmp(other) <= 0; }
    bool operator>(const Oligo& other) const { return cmp(other) > 0; }
    bool operator>=(const Oligo& other) const { return cmp(other) >= 0; }

    //TODO implement spaceship (<=>) operator

    /**
     * @brief Subscript operator.
     * @param idx The index of the character.
     * @return The character at the specified index or '\0' if out of range.
     */
    int operator[](size_t idx) const {
        return static_cast<int>((data_block >> (2 * (bp() - idx - 1))) & 0x3);
    }

    /**
     * @brief Create a new oligo by slicing the current oligo.
     * @param start The starting index of the slice.
     * @param end The ending index of the slice.
     * @return An Oligo object
     */
    Oligo slice(size_t start, size_t end) const {
        // Ensure valid indices
        start = std::min(start, bp() - 1);
        end = std::min(end, bp() - 1);
        end = std::max(start, end);

        auto new_bp = end - start + 1;
        uint64_t omask = (1ULL << (2 * new_bp)) - 1;

        return Oligo(new_bp, (data_block >> (2 * (bp() - 1 - end))) & omask);
    }

    /**
     * @brief Append another oligo to the current oligo.
     * @param other The other oligo to append.
     * @return True if the append operation was successful, false otherwise.
     */
    bool append(const Oligo& other) {
        if (bp() + other.bp() > MAX_BP)
            return false;

        for (size_t i = 0; i < other.bp(); ++i) {
            int nt = other[i];
            data_block = (data_block << 2) | static_cast<uint64_t>(nt);
        }
        basepairs += other.bp();
        return true;
    }

    /**
     * @brief Write binary data to file
     * @param of the ofstream (input file) to write to
     */
    void write_bin(std::ofstream &of) {
        char arr[8];
        for (int i = 0; i < 8; i++)
            arr[i] = static_cast<char>((data() >> (i * 8)) & 0xFF);
        of.write(arr, sizeof(uint64_t));
    }

#ifdef HAVE_RS_MODULE
    /**
     * @brief Function to encode an Oligo using libfec Reed-Solomon Error Corretion
     * @param oligo The oligo to be encoded
     */
    void encode() const {
        unsigned char data[8];    // Data to be encoded
        unsigned char parity[8];  // Parity data after encoding

        // Convert the oligo into an appropriate format for encoding
        for (int i = 0; i < 8; i++)
            data[i] = static_cast<char>((data_block >> (i * 8)) & 0xFF);

        // Perform encoding using libfec
        encode_rs_char(init_rs_char(8, 0x187, 0, 1, 32, 0), data, parity);

        // TODO: Handle the encoded data (e.g., storing it or transmitting)
        // For now, let's print the encoded data for demonstration purposes
        std::cout << "Encoded data: ";
        for (size_t i = 0; i < 8; i++)
            std::cout << static_cast<int>(parity[i]) << " ";
        std::cout << std::endl;
    }

    /**
     * @brief Function to decode an Oligo using libfec
     */
    void decode() const {
        unsigned char receivedData[MAX_BP];  // Received data to be decoded
        int erasures[MAX_BP];                // Array to store erasure positions (if any)

        // Simulating received data (encoded data from transmission)
        // Replace this with the actual received data
        for (size_t i = 0; i < basepairs; ++i)
            receivedData[i] = static_cast<unsigned char>((data_block >> (2 * (basepairs - i - 1))) & 0x3);

        // Simulating erasures (missing/corrupted positions)
        // Replace this with the actual erasure positions
        int numErasures = 0;  // Number of erasures
                              // ...

                              // Perform decoding using libfec
        int decodeResult = decode_rs_char(init_rs_char(8, 0x187, 0, 1, 32, 0), receivedData, erasures, numErasures);

        // Check the decode result
        if (decodeResult < 0) {
            std::cout << "Decoded data: ";
            for (size_t i = 0; i < basepairs; ++i)
                std::cout << static_cast<int>(receivedData[i]) << " ";
            std::cout << std::endl;
        }
        else {
            std::cout << "Decoding failed." << std::endl;
        }
    }
#endif
};

