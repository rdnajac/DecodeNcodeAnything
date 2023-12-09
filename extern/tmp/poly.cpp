#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

template <typename T>
using Poly = std::vector<T>;

/**
 * @brief Create a polynomial using a given degree and array of coefficients.
 * @tparam T Type of coefficients in the polynomial.
 * @param n Degree of the polynomial.
 * @param data Array of coefficients.
 * @return Created polynomial.
 */
template <typename T>
Poly<T> Poly_Create(int n, const T* coef) {
    return Poly<T>(n, coef);
}

/**
 * @brief Scale a polynomial by a constant factor.
 *
 * @tparam T Type of coefficients in the polynomial.
 * @param out Resultant polynomial.
 * @param in Input polynomial.
 * @param scale Constant factor.
 * @param gf Galois Field.
 */
template <typename T>
void Poly_Add(Poly<T>& out, const Poly<T>& a, const Poly<T>& b) {
    int n = std::max(a.size(), b.size());
    out.resize(n);

    std::transform(a.rbegin(), a.rend(), b.rbegin(), out.rbegin(), std::bit_xor<T>());
}

/**
 * @brief Scale a polynomial by a constant factor.
 * @tparam T Type of coefficients in the polynomial.
 * @param out Resultant polynomial.
 * @param in Input polynomial.
 * @param scale Constant factor.
 * @param gf Galois Field.
 */
template <typename T>
void Poly_Scale(Poly<T>& out, const Poly<T>& in, T scale, const GaloisField<T>& gf) {
    if (&out != &in)
        out = in;

    for (auto& coef : out)
        coef = gf.mult(coef, scale);
}

/**
 * @brief Multiply two polynomials.
 * @tparam T Type of coefficients in the polynomial.
 * @param out Resultant polynomial.
 * @param a First polynomial.
 * @param b Second polynomial.
 * @param gf Galois Field.
 */
template <typename T>
void Poly_Mult(Poly<T>& out, const Poly<T>& a, const Poly<T>& b, const GaloisField<T>& gf) {
    int n = a.size() + b.size() - 1;
    out.resize(n, 0);

    for (int i = 0; i < a.size(); ++i)
        for (int j = 0; j < b.size(); ++j)
            out[i + j] ^= gf.mult(a[i], b[j]);
}

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
void Poly_Div(Poly<T>& result, Poly<T>& quotient, Poly<T>& remainder, const Poly<T>& a, const Poly<T>& b, const GaloisField<T>& gf) {
    int n = a.size();
    result.resize(n);

    Poly<T> temp(a);

    T normalizer = b[0];
    for (int i = 0; i < n - b.size() + 1; ++i) {
        temp[i] = gf.div(temp[i], normalizer);

        T coef = temp[i];
        if (coef != 0)
            for (int j = 1; j < b.size(); ++j)
                if (b[j] != 0)
                    temp[i + j] ^= gf.mult(b[j], coef);
    }
    result = temp;

    int separator = n - b.size() + 1;
    quotient.assign(temp.begin(), temp.begin() + separator);
    remainder.assign(temp.begin() + separator, temp.end());
}

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
T Poly_Eval(const Poly<T>& poly, T x, const GaloisField<T>& gf) {
    T y = poly[0];
    for (auto& coef : poly)
        y = gf.mult(y, x) ^ coef;
    return y;
}

/**
 * @brief Perform Chien search on a polynomial.
 *
 * @tparam T Type of coefficients in the polynomial.
 * @param out Vector to store the roots found.
 * @param poly Polynomial for Chien search.
 * @param max Maximum number of roots to search for.
 * @param gf Galois Field.
 */
template <typename T>
void Poly_ChienSearch(std::vector<unsigned int>& out, const Poly<T>& poly, int max, const GaloisField<T>& gf) {
    Poly<T> temp(poly);

    for (int i = 0; i < max; ++i) {
        T sum = 0;
        for (auto& coef : temp) {
            sum ^= coef;
            coef = gf.mult(coef, gf.powTable[poly.size() - &coef - 1]);
        }

        if (sum == 0)
            out.push_back(i);
    }
}

/**
 * @brief Pad a polynomial with zeros on the left and right.
 * @tparam T Type of coefficients in the polynomial.
 * @param poly Polynomial to pad.
 * @param left Number of zeros to add on the left.
 * @param right Number of zeros to add on the right.
 */
template <typename T>
void Poly_Pad(Poly<T>& poly, int left, int right) {
    poly.resize(poly.size() + left + right, 0);
    std::rotate(poly.rbegin(), poly.rbegin() + right, poly.rend());
}

/**
 * @brief Trim a polynomial by removing leading and trailing zeros.
 *
 * @tparam T Type of coefficients in the polynomial.
 * @param poly Polynomial to trim.
 * @param left Number of leading zeros to remove.
 * @param right Number of trailing zeros to remove.
 */
template <typename T>
void Poly_Trim(Poly<T>& poly, int left, int right) {
    poly.erase(poly.begin(), poly.begin() + left);
    poly.erase(poly.end() - right, poly.end());
}

/**
 * @brief Append two polynomials.
 * @tparam T Type of coefficients in the polynomial.
 * @param out Resultant polynomial.
 * @param a First polynomial.
 * @param b Second polynomial.
 */
template <typename T>
void Poly_Append(Poly<T>& out, const Poly<T>& a, const Poly<T>& b) {
    out = a;
    out.insert(out.end(), b.begin(), b.end());
}

/**
 * @brief Reverse the coefficients of a polynomial.
 * @tparam T Type of coefficients in the polynomial.
 * @param out Resultant polynomial.
 * @param in Input polynomial.
 */
template <typename T>
void Poly_Reverse(Poly<T>& out, const Poly<T>& in) {
    out = in;
    std::reverse(out.begin(), out.end());
}

/**
 * @brief Template class representing a polynomial.
 * @tparam T Type of coefficients in the polynomial.
 */
template <typename T>
class Poly {
public:

    std::vector<T> coef;  ///< Vector of coefficients.

    /**
     * @brief Parameterized constructor for the Poly class.
     * @param n Degree of the polynomial.
     * @param coef Array of coefficients.
     */
    Poly(int n, T* coef) : coef(coef, coef + n) {}

    /**
     * @brief Set the coefficients by copying from an array.
     * @param n Degree of the polynomial.
     * @param coef Array of coefficients.
     */
    void setCopy(int n, const T* coef) { this->coef.assign(coef, coef + n); }

    /**
     * @brief Same as setCopy but retained for compatibility
     * @param n Degree of the polynomial.
     * @param coef Array of coefficients.
     */
    void setRef(int n, const T* coef) { coef.assign(coef, coef + n); }

    /**
     * @brief Print the polynomial.
     */
    void print() const {
        std::cout << "Poly(n=" << coef.size() << ")";
        if (!coef.empty()) {
            std::cout << "[";
            for (const auto& c : coef) {
                std::cout << std::hex << std::setw(3) << static_cast<T>(c);
                if (&c != &coef.back()) std::cout << ", ";
            }
            std::cout << "]" << std::dec << std::endl;
        }
    }
};

