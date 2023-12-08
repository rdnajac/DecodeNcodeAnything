/**
 * @file utils.hpp
 * @brief A collection of helper functions
 */
#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string>
#include <span>
#include<optional>
#include <unordered_map>
#include<ranges>
#include<numeric>
/**
 * @brief Enum representing nucleotide types.
 */
enum class Nucleotide {
    A = 0, /**< Adenine */
    C = 1, /**< Cytosine*/
    G = 2, /**< Guanine */
    T = 3  /**< Thymine */
};

/**
 * @brief String representation of nucleotides for conversion.
 */
const std::string nucleotideStr = "ACGT";

/**
 * @brief Convert numeric value of a nucleotide (nt) to its string value.
 * @param nt Numeric value of a nucleotide.
 * @return String representation of the nucleotide.
 */
std::string nt2string(int nt);

/**
 * @brief Convert character value of a nucleotide (nt) to its numeric value.
 * @param nt Character representation of a nucleotide.
 * @return Numeric value of the nucleotide, or -1 if not recognized.
 */
std::optional<int> char2nt(char nt);

/**
 * @brief Convert string value of a nucleotide (nt) to its numeric value.
 * @param nt String representation of a nucleotide.
 * @return Numeric value of the nucleotide, or -1 if not recognized.
 */
int string2nt(const std::string& nt);

/**
 * @brief Calculate the GC content of a given oligonucleotide sequence.
 * @param sequence The input oligonucleotide sequence.
 * @return The GC content as a double between 0 and 1.
 */
double calculateGCContent(const std::string& sequence);

/**
 * @brief Get the maximum homopolymer length in a given oligonucleotide sequence.
 * @param sequence The input oligonucleotide sequence.
 * @return The maximum homopolymer length.
 */
int calculateMaxHomopolymerLen(const std::string& sequence);

/**
 * @brief Generates the reverse complement of a DNA sequence.
 * @param dna The input DNA sequence.
 * @return The reverse complement of the input DNA sequence.
 */
std::string revcom(const std::string& dna);

/**
 * @brief Calculates the Levenshtein distance between two strings.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return The Levenshtein distance between str1 and str2.
 */
int levenshtein_distance(const std::string& str1, const std::string& str2);

/**
 * @brief Matches two strings allowing '?' for matching any character.
 * @param p The first string.
 * @param q The second string.
 * @param maxdist The maximum allowed distance.
 * @return True if the strings match within the specified maximum distance, false otherwise.
 */
bool Match(const std::string& p, const std::string& q, int maxdist);

/**
 * @brief Finds the minimum of two pairs.
 * @param a First integer.
 * @param b Second integer.
 * @param aa First integer in the pair.
 * @param bb Second integer in the pair.
 * @return The pair with the minimum first integer.
 */
std::pair<int, int> min2(int a, int b, int aa, int bb);

/**
 * @brief Updates distances and minimum distance based on current values.
 * @param fj1 Value of f[j - 1] in the last iteration.
 * @param lj1 Value of l[j - 1] in the last iteration.
 * @param cbMismatch Whether there is a mismatch between characters.
 * @param mdist Current minimum distance.
 * @param mn Current minimum value.
 * @param ln Current length.
 */
void update_distances(int& fj1, int& lj1, int cbMismatch, int& mdist, int& mn, int& ln);

/**
 * @brief Finds a substring in a string with up to maxdist errors allowed.
 * @param s The original string.
 * @param subseq The substring to find.
 * @param maxdist The maximum allowed distance.
 * @param pos Output parameter for the position in the original string.
 * @param length Output parameter for the length of the found substring.
 * @return True if the substring is found within the specified maximum distance, false otherwise.
 */
bool Find(const std::string& s, const std::string& subseq, int maxdist, int& pos, int& length);

/**
 * @brief Finds the position and length of the longest prefix with up to maxdist errors.
 * @param s The original string.
 * @param prefix The prefix to find.
 * @param maxdist The maximum allowed distance.
 * @return A pair containing the position and length of the found prefix.
 */
std::pair<int, int> FindPrefix(const std::string& s, const std::string& prefix, int maxdist);

/**
 * @brief Finds the position and length of the longest suffix with up to maxdist errors.
 * @param s The original string.
 * @param suffix The suffix to find.
 * @param maxdist The maximum allowed distance.
 * @return A pair containing the position and length of the found suffix.
 */
std::pair<int, int> FindSuffix(const std::string& s, const std::string& suffix, int maxdist);

/**
 * @brief Computes the difference between two strings using dynamic programming.
 * @param from The source string.
 * @param to The target string.
 * @return A pair containing the minimum edit distance and the edit actions to transform 'from' into 'to'.
 */
std::pair<int, std::string> Diff(const std::string& from, const std::string& to);

#endif
