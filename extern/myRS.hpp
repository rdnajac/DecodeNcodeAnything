#pragma once

#include <iostream>
#include <vector>
#include <iomanip>

/**
 * @brief Template class representing the Reed-Solomon error correction code.
 * @tparam T Type of elements in the Reed-Solomon code.
 */
template <typename T>
class ReedSolomon {
private:
    GaloisField<T> gf;  ///< Galois Field used for Reed-Solomon operations.

public:
    /**
     * @brief Constructor for the ReedSolomon class.
     * @param fieldPower Power of the Galois Field.
     */
    ReedSolomon(int fieldPower);

    /**
     * @brief Generate the generator polynomial for Reed-Solomon codes.
     * @param out Output polynomial representing the generator.
     * @param nsym Number of symbols in the error-correction code.
     */
    void createGenerator(Poly<T>& out, int nsym);

    /**
     * @brief Encode a message using Reed-Solomon.
     * @param out Output array for the encoded message.
     * @param data Input array containing the original message.
     * @param k Number of symbols in the original message.
     * @param nsym Number of symbols in the error-correction code.
     */
    void encode(T* out, T* data, int k, int nsym);

    /**
     * @brief Calculate syndromes for error detection.
     * @param out Output polynomial containing syndromes.
     * @param msg Input polynomial representing the received message.
     * @param nsym Number of symbols in the error-correction code.
     */
    void calcSyndromes(Poly<T>& out, const Poly<T>& msg, int nsym);

    /**
     * @brief Check if syndromes indicate errors in the received message.
     * @param synd Input polynomial containing syndromes.
     * @return True if errors are detected, false otherwise.
     */
    bool checkSyndromes(const Poly<T>& synd);

    /**
     * @brief Find the error locator polynomial.
     * @param out Output polynomial representing the error locator.
     * @param synd Input polynomial containing syndromes.
     * @param nsym Number of symbols in the error-correction code.
     * @param eraseLoc Array of erasure positions.
     * @param eraseCount Number of erasures.
     * @return True if the error locator is found, false otherwise.
     */
    bool findErrorLocator(Poly<T>& out, const Poly<T>& synd, int nsym, const Poly<T>* eraseLoc, int eraseCount);

    /**
     * @brief Find the positions of errors in the received message.
     * @param out Output vector containing error positions.
     * @param errLoc Input polynomial representing the error locator.
     * @param n Number of symbols in the error-correction code.
     * @return True if errors are found, false otherwise.
     */
    bool findErrors(std::vector<unsigned int>& out, const Poly<T>& errLoc, int n);

    /**
     * @brief Forney algorithm to calculate syndromes at error positions.
     * @param out Output polynomial containing syndromes at error positions.
     * @param synd Input polynomial containing syndromes.
     * @param pos Vector of error positions.
     * @param n Number of symbols in the error-correction code.
     */
    void forneySyndromes(Poly<T>& out, const Poly<T>& synd, const std::vector<unsigned int>& pos, int n);

    /**
     * @brief Decode a Reed-Solomon encoded message.
     * @param wholeOut Output array for the fully decoded message.
     * @param out Output array for the partially decoded message.
     * @param data Input array containing the received message.
     * @param k Number of symbols in the original message.
     * @param nsym Number of symbols in the error-correction code.
     * @param erasePos Vector of erasure positions.
     * @param debug Enable debug mode.
     * @return True if decoding is successful, false otherwise.
     */
    bool decode(T* wholeOut, T* out, T* data, int k, int nsym, const std::vector<unsigned int>& erasePos, bool debug);
};
#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

/**
 * @brief Class representing a Galois Field of order 2^fieldPower.
 * @tparam T Type of elements in the Galois Field.
 */
template <typename T>
class GaloisField {
private:
    const int fieldPower;   ///< Power of the field (2^fieldPower).
    const T characteristic; ///< Characteristic of the field (2^fieldPower).
    const T primitivePoly;  ///< Primitive polynomial for the field.

public:
    T* powTable;            ///< Table for exponentiation.
    T* logTable;            ///< Table for logarithms.

    /**
     * @brief Constructor for the GaloisField class.
     * @param fieldPower Power of the Galois Field.
     */
    GaloisField(int fieldPower);

    /**
     * @brief Destructor for the GaloisField class.
     */
    ~GaloisField();

    /**
     * @brief Multiply two elements in the Galois Field without using lookup tables.
     * @param a First element.
     * @param b Second element.
     * @return Result of the multiplication.
     */
    T multNoLUT(T a, T b);

    /**
     * @brief Multiply two elements in the Galois Field using lookup tables.
     * @param a First element.
     * @param b Second element.
     * @return Result of the multiplication.
     */
    T mult(T a, T b);

    /**
     * @brief Divide two elements in the Galois Field.
     * @param a Numerator.
     * @param b Denominator.
     * @return Result of the division.
     */
    T div(T a, T b);

    /**
     * @brief Calculate the power of an element in the Galois Field.
     * @param x Base.
     * @param power Exponent.
     * @return Result of the exponentiation.
     */
    T pow(T x, T power);

    /**
     * @brief Calculate the multiplicative inverse of an element in the Galois Field.
     * @param x Element to find the inverse of.
     * @return Multiplicative inverse.
     */
    T inv(T x);

    /**
     * @brief Calculate the square root of an element in the Galois Field.
     * @param x Element to find the square root of.
     * @return Square root.
     */
    T sqrt(T x);
};


template <typename T>
using Poly = std::vector<T>;

/**
 * @brief Create a polynomial using a given degree and array of coefficients.
 * @tparam T Type of coefficients in the polynomial.
 * @param n Degree of the polynomial.
 * @param coef Array of coefficients.
 * @return Created polynomial.
 */
template <typename T>
Poly<T> Poly_Create(int n, const T* coef);

/**
 * @brief Add two polynomials.
 * @tparam T Type of coefficients in the polynomial.
 * @param out Resultant polynomial.
 * @param a First polynomial.
 * @param b Second polynomial.
 */
template <typename T>
void Poly_Add(Poly<T>& out, const Poly<T>& a, const Poly<T>& b);

/**
 * @brief Scale a polynomial by a constant factor.
 * @tparam T Type of coefficients in the polynomial.
 * @param out Resultant polynomial.
 * @param in Input polynomial.
 * @param scale Constant factor.
 * @param gf Galois Field.
 */
template <typename T>
void Poly_Scale(Poly<T>& out, const Poly<T>& in, T scale, const GaloisField<T>& gf);

/**
 * @brief Multiply two polynomials.
 * @tparam T Type of coefficients in the polynomial.
 * @param out Resultant polynomial.
 * @param a First polynomial.
 * @param b Second polynomial.
 * @param gf Galois Field.
 */
template <typename T>
void Poly_Mult(Poly<T>& out, const Poly<T>& a, const Poly<T>& b, const GaloisField<T>& gf);

/**
 * @brief Divide two polynomials.
 * @tparam T Type of coefficients in the polynomial.
 * @param result Resultant polynomial after division.
 * @param quotient Quotient polynomial after division.
 * @param remainder Remainder polynomial after division.
 * @param a Numerator polynomial.
 * @param b Denominator polynomial.
 * @param gf Galois Field.
 */
template <typename T>
void Poly_Div(Poly<T>& result, Poly<T>& quotient, Poly<T>& remainder, const Poly<T>& a, const Poly<T>& b, const GaloisField<T>& gf);

/**
 * @brief Evaluate a polynomial at a given point.
 * @tparam T Type of coefficients in the polynomial.
 * @param poly Polynomial to evaluate.
 * @param x Point at which to evaluate the polynomial.
 * @param gf Galois Field.
 * @return Result of the evaluation.
 */
template <typename T>
T Poly_Eval(const Poly<T>& poly, T x, const GaloisField<T>& gf);

/**
 * @brief Perform Chien search on a polynomial.
 * @tparam T Type of coefficients in the polynomial.
 * @param out Vector to store the roots found.
 * @param poly Polynomial for Chien search.
 * @param max Maximum number of roots to search for.
 * @param gf Galois Field.
 */
template <typename T>
void Poly_ChienSearch(std::vector<unsigned int>& out, const Poly<T>& poly, int max, const GaloisField<T>& gf);

/**
 * @brief Pad a polynomial with zeros on the left and right.
 * @tparam T Type of coefficients in the polynomial.
 * @param poly Polynomial to pad.
 * @param left Number of zeros to add on the left.
 * @param right Number of zeros to add on the right.
 */
template <typename T>
void Poly_Pad(Poly<T>& poly, int left, int right);

/**
 * @brief Trim a polynomial by removing leading and trailing zeros.
 * @tparam T Type of coefficients in the polynomial.
 * @param poly Polynomial to trim.
 * @param left Number of leading zeros to remove.
 * @param right Number of trailing zeros to remove.
 */
template <typename T>
void Poly_Trim(Poly<T>& poly, int left, int right);

/**
 * @brief Append two polynomials.
 * @tparam T Type of coefficients in the polynomial.
 * @param out Resultant polynomial.
 * @param a First polynomial.
 * @param b Second polynomial.
 */
template <typename T>
void Poly_Append(Poly<T>& out, const Poly<T>& a, const Poly<T>& b);

/**
 * @brief Reverse the coefficients of a polynomial.
 * @tparam T Type of coefficients in the polynomial.
 * @param out Resultant polynomial.
 * @param in Input polynomial.
 */
template <typename T>
void Poly_Reverse(Poly<T>& out, const Poly<T>& in);

template <typename T>
class Poly {
public:
    std::vector<T> coef;  ///< Vector of coefficients.

    /**
     * @brief Parameterized constructor for the Poly class.
     * @param n Degree of the polynomial.
     * @param coef Array of coefficients.
     */
    Poly(int n, T* coef);

    /**
     * @brief Set the coefficients of the polynomial by copying from another polynomial.
     * @param n Degree of the polynomial.
     * @param coef Array of coefficients.
     */
    void setCopy(int n, const T* coef);

    /**
     * @brief Set the coefficients of the polynomial by referencing an existing array.
     * @param n Degree of the polynomial.
     * @param coef Array of coefficients.
     */
    void setRef(int n, const T* coef);

    /**
     * @brief Print the coefficients of the polynomial.
     */
    void print() const;
};


/**
 * @brief Template class representing the Reed-Solomon error correction code.
 * @tparam T Type of elements in the Reed-Solomon code.
 */
template <typename T>
class ReedSolomon {
private:
    GaloisField<T> gf;  ///< Galois Field used for Reed-Solomon operations.

public:
    /**
     * @brief Constructor for the ReedSolomon class.
     * @param fieldPower Power of the Galois Field.
     */
    ReedSolomon(int fieldPower);

    /**
     * @brief Generate the generator polynomial for Reed-Solomon codes.
     * @param out Output polynomial representing the generator.
     * @param nsym Number of symbols in the error-correction code.
     */
    void createGenerator(Poly<T>& out, int nsym);

    /**
     * @brief Encode a message using Reed-Solomon.
     * @param out Output array for the encoded message.
     * @param data Input array containing the original message.
     * @param k Number of symbols in the original message.
     * @param nsym Number of symbols in the error-correction code.
     */
    void encode(T* out, T* data, int k, int nsym);

    /**
     * @brief Calculate syndromes for error detection.
     * @param out Output polynomial containing syndromes.
     * @param msg Input polynomial representing the received message.
     * @param nsym Number of symbols in the error-correction code.
     */
    void calcSyndromes(Poly<T>& out, const Poly<T>& msg, int nsym);

    /**
     * @brief Check if syndromes indicate errors in the received message.
     * @param synd Input polynomial containing syndromes.
     * @return True if errors are detected, false otherwise.
     */
    bool checkSyndromes(const Poly<T>& synd);

    /**
     * @brief Find the error locator polynomial.
     * @param out Output polynomial representing the error locator.
     * @param synd Input polynomial containing syndromes.
     * @param nsym Number of symbols in the error-correction code.
     * @param eraseLoc Array of erasure positions.
     * @param eraseCount Number of erasures.
     * @return True if the error locator is found, false otherwise.
     */
    bool findErrorLocator(Poly<T>& out, const Poly<T>& synd, int nsym, const Poly<T>* eraseLoc, int eraseCount);

    /**
     * @brief Find the positions of errors in the received message.
     * @param out Output vector containing error positions.
     * @param errLoc Input polynomial representing the error locator.
     * @param n Number of symbols in the error-correction code.
     * @return True if errors are found, false otherwise.
     */
    bool findErrors(std::vector<unsigned int>& out, const Poly<T>& errLoc, int n);

    /**
     * @brief Forney algorithm to calculate syndromes at error positions.
     * @param out Output polynomial containing syndromes at error positions.
     * @param synd Input polynomial containing syndromes.
     * @param pos Vector of error positions.
     * @param n Number of symbols in the error-correction code.
     */
    void forneySyndromes(Poly<T>& out, const Poly<T>& synd, const std::vector<unsigned int>& pos, int n);

    /**
     * @brief Decode a Reed-Solomon encoded message.
     * @param wholeOut Output array for the fully decoded message.
     * @param out Output array for the partially decoded message.
     * @param data Input array containing the received message.
     * @param k Number of symbols in the original message.
     * @param nsym Number of symbols in the error-correction code.
     * @param erasePos Vector of erasure positions.
     * @param debug Enable debug mode.
     * @return True if decoding is successful, false otherwise.
     */
};
