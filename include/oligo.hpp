#ifndef _OLIGO_HPP
#define _OLIGO_HPP

#include <string>
#include <cstdint>

/**
 * @brief Represents an oligonucleotide (DNA sequence).
 */
class Oligo {
private:
    int length;
    uint64_t data_block;

public:
    /**
     * @brief Default constructor.
     */
    Oligo();

    /**
     * @brief Parameterized constructor.
     * @param len The length of the oligonucleotide.
     * @param val The value of the oligonucleotide.
     */
    Oligo(size_t len, uint64_t val);
    Oligo(const std::string& s);

    /**
     * @brief Destructor.
     */
    ~Oligo();

    /**
     * @brief Get the length of the oligonucleotide.
     * @return The length of the sequence.
     */
    size_t len() const;

    /**
     * @brief Get the data block of the oligonucleotide.
     * @return The data block.
     */
    uint64_t getDataBlock() const;

    /**
     * @brief Get the string representation of the data_block.
     * @return The string representation of the data_block.
     */
    std::string seq() const;

    /**
     * @brief Create a new short oligo object with the specified length.
     * @param olen The length of the oligonucleotide.
     * @return A pointer to the new Oligo object or nullptr if olen > 32.
     */
    static Oligo* newOligo(size_t olen);

    /**
     * @brief Create a new short oligo object with the specified length and value.
     * @param olen The length of the oligonucleotide.
     * @param val The value of the oligonucleotide.
     * @return A pointer to the new Oligo object or nullptr if olen > 32.
     */
    static Oligo* val(size_t olen, uint64_t val);

    /**
     * @brief Convert a string representation of an oligo to an Oligo object.
     * @param s The string representation of the oligo.
     * @return A pair containing a pointer to the new Oligo object and a boolean indicating if the conversion was successful.
     */
    static std::pair<Oligo*, bool> fromString(const std::string& s);

    /**
     * @brief Create a copy of the given oligo.
     * @param o The oligo to copy.
     * @return A pair containing a pointer to the new Oligo object and a boolean indicating if the copy was successful.
     */
    static std::pair<Oligo*, bool> copy(const Oligo& o);

    /**
     * @brief Compare the oligo with another oligo.
     * @param other The other oligo to compare.
     * @return -1 if the oligo is less than the other, 0 if equal, and 1 if greater.
     */
    int cmp(const Oligo& other) const;

    /**
     * @brief Check if the oligo is equal to another oligo.
     * @param other The other oligo to compare.
     * @return True if the oligos are equal, false otherwise.
     */
    bool operator==(const Oligo& other) const;

    /**
     * @brief Check if the oligo is less than another oligo.
     * @param other The other oligo to compare.
     * @return True if the oligo is less than the other, false otherwise.
     */
    bool operator<(const Oligo& other) const;

    /**
     * @brief Check if the oligo is greater than another oligo.
     * @param other The other oligo to compare.
     * @return True if the oligo is greater than the other, false otherwise.
     */
    bool operator>(const Oligo& other) const;

    /**
     * @brief Check if the oligo is less than or equal to another oligo.
     * @param other The other oligo to compare.
     * @return True if the oligo is less than or equal to the other, false otherwise.
     */
    bool operator<=(const Oligo& other) const;

    /**
     * @brief Check if the oligo is greater than or equal to another oligo.
     * @param other The other oligo to compare.
     * @return True if the oligo is greater than or equal to the other, false otherwise.
     */
    bool operator>=(const Oligo& other) const;

    /**
     * @brief Check if the oligo is not equal to another oligo.
     * @param other The other oligo to compare.
     * @return True if the oligos are not equal, false otherwise.
     */
    bool operator!=(const Oligo& other) const;

    /**
     * @brief Prefix increment operator.
     * @return A reference to the incremented oligo.
     */
    Oligo& operator++();

    /**
     * @brief Postfix increment operator.
     * @param int Dummy parameter for postfix increment.
     * @return The oligo before incrementing.
     */
    Oligo operator++(int);

    /**
     * @brief Subscript operator.
     * @param idx The index of the character.
     * @return The character at the specified index or '\0' if out of range.
     */
    int operator[](size_t idx) const;

    /**
     * @brief Create a new oligo by slicing the current oligo.
     * @param start The starting index of the slice.
     * @param end The ending index of the slice.
     * @return A pointer to the new Oligo object or nullptr if the slice is invalid.
     */
    Oligo* slice(int start, int end);

    /**
     * @brief Create a clone of the current oligo.
     * @return A pointer to the new Oligo object.
     */
    Oligo* clone();

    /**
     * @brief Append another oligo to the current oligo.
     * @param other The other oligo to append.
     * @return True if the append operation was successful, false otherwise.
     */
    bool append(const Oligo& other);

    /**
     * @brief Get the uint64_t representation of the oligo.
     * @return The uint64_t representation of the oligo.
     */
    uint64_t uint64() const;
};

#endif // _OLIGO_HPP

