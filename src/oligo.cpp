#include <string>
#include <cstdint>
#include <algorithm>
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
        for (char c : s)
            data_block = (data_block << 2) | (s.length() > MAX_BP ? 0 : char2nt(c));
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
   /* int cmp(const Oligo& other) const {
        int bpComparison = std::minmax(bp(), other.bp()).first;
        if (bpComparison != 0) return bpComparison;

        int dataComparison = std::minmax(data_block, other.data()).first;
        return dataComparison;
    }*/
    int cmp(const Oligo& other) const {
        // Compare lengths of sequences
        if (bp() != other.bp()) {
            return (bp() < other.bp()) ? -1 : 1;
        }

        // Compare sequences character by character
        for (size_t i = 0; i < bp(); ++i) {
            int nt1 = (*this)[i];
            int nt2 = other[i];

            if (nt1 != nt2) {
                return (nt1 < nt2) ? -1 : 1;
            }
        }

        return 0;  // Sequences are identical
    }
    bool operator==(const Oligo& other) const { return cmp(other) == 0; }
    bool operator!=(const Oligo& other) const { return cmp(other) != 0; }
    bool operator<(const Oligo& other) const { return cmp(other) < 0; }
    bool operator<=(const Oligo& other) const { return cmp(other) <= 0; }
    bool operator>(const Oligo& other) const { return cmp(other) > 0; }
    bool operator>=(const Oligo& other) const { return cmp(other) >= 0; }

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
     * @return A new Oligo object or Oligo() if the slice is invalid.
     */
 /*   Oligo slice(size_t start, size_t end) const {
        end = (end <= 0) ? bp() - end : std::min(end, bp());
        end = (end < 0) ? 0 : end;
        start = (start > bp() || start > end) ? 0 : start;

        auto new_bp = end - start;
        uint64_t omask = (1ULL << (2 * new_bp)) - 1;

        return Oligo(new_bp, (data_block >> (2 * (bp() - end))) & omask);
    }*/

    Oligo slice(size_t start, size_t end) const {
        // Handle negative indices
        if (start < 0) start = 0;
        if (end < 0) end = 0;

        // Adjust indices within valid range
        if (start >= bp()) start = bp() - 1;
        if (end >= bp()) end = bp() - 1;
        if (end < start) end = start;

        auto new_bp = end - start + 1;
        uint64_t omask = (1ULL << (2 * new_bp)) - 1;

        return Oligo(new_bp, (data_block >> (2 * (bp() - end - 1))) & omask);
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
};

