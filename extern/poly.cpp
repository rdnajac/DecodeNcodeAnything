#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "primes.hpp"

template <typename T>
concept UnsignedInteger = std::is_unsigned_v<T> &&
                         (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);

template <typename T>
class Poly {
public:
    std::vector<T> coef;  ///< Vector of coefficients.

    /**
     * @brief Default constructor for a polynomial.
     */
    Poly() = default;

    /**
     * @brief Parameterized constructor for the Poly class.
     * @param n Degree of the polynomial.
     * @param initialValue Initial value for coefficients.
     */
    Poly(int n, T initialValue = T{}) : coef(n, initialValue) {}

    /**
     * @brief Copy constructor using the assignment operator.
     * @param other Another Poly object to copy from.
     */
    Poly(const Poly<T>& other) : coef(other.coef) {}

    /**
     * @brief Get the size of the coefficient vector.
     * @return Size of the coefficient vector.
     */
    size_t size() const { return coef.size() }

    /**
     * @brief Subscript operator to access coefficients by index.
     * @param index Index of the coefficient to access.
     * @return Reference to the coefficient at the specified index.
     */
    T& operator[](size_t index) { return coef[index]; }

    /**
     * @brief Const subscript operator to access coefficients by index.
     * @param index Index of the coefficient to access.
     * @return Const reference to the coefficient at the specified index.
     */
    const T& operator[](size_t index) const { return coef[index]; }

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

    /**
     * @brief Reverse the coefficients of the polynomial.
     */
    void reverse() { std::reverse(coef.begin(), coef.end()); }

    /**
     * @brief Add two polynomials and store the result in this polynomial.
     * @param other The polynomial to add.
     */
    void Add(const Poly<T>& other) {
        int n = std::max(coef.size(), other.coef.size());
        coef.resize(n);

        std::transform(coef.rbegin(), coef.rend(), other.coef.rbegin(), coef.rbegin(), std::plus<T>());
    }

    /**
     * @brief Scale a polynomial by a constant factor.
     * @param scale Constant factor.
     * @param gf Galois Field.
     */
    void Scale(T scale, const GaloisField<T>& gf) {
        if (&out != &in)
            out = in;
        for (auto& coef : out)
            coef = gf.mult(coef, scale);
    }

    /**
     * @brief Multiply this polynomial with another polynomial and store the result in this polynomial.
     * @param other The other polynomial to multiply with.
     * @param gf Galois Field.
     */
    void Mult(const Poly<T>& other, const GaloisField<T>& gf) {
        int n = coef.size() + other.coef.size() - 1;
        coef.resize(n, 0);

        for (int i = 0; i < coef.size(); ++i)
            for (int j = 0; j < other.coef.size(); ++j)
                coef[i + j] ^= gf.mult(coef[i], other.coef[j]);
    }

    /**
     * @brief Divide this polynomial by another polynomial.
     * @param other The divisor polynomial.
     * @param gf Galois Field.
     */
    void Div(const Poly<T>& other, const GaloisField<T>& gf) {
        int n = coef.size();
        Poly<T> temp(*this);

        T normalizer = other.coef[0];
        for (int i = 0; i < n - other.coef.size() + 1; ++i) {
            temp.coef[i] = gf.div(temp.coef[i], normalizer);

            T coef = temp.coef[i];
            if (coef != 0)
                for (int j = 1; j < other.coef.size(); ++j)
                    if (other.coef[j] != 0)
                        temp.coef[i + j] ^= gf.mult(other.coef[j], coef);
        }

        int separator = n - other.coef.size() + 1;
        coef.assign(temp.coef.begin(), temp.coef.begin() + separator);
    }

    /**
     * @brief Evaluate the polynomial for a given value of x using a specified Galois Field.
     * @param x Value for which the polynomial is evaluated.
     * @param gf Galois Field.
     * @return Result of the polynomial evaluation.
     */
    T Eval(T x, const GaloisField<T>& gf) const {
        T y = coef[0];
        for (auto it = std::next(coef.begin()); it != coef.end(); ++it)
            y = gf.mult(y, x) ^ *it;
        return y;
    }
    /**
     * @brief Pad the polynomial with zeros on the left and right.
     * @param left Number of zeros to add on the left.
     * @param right Number of zeros to add on the right.
     */
    void Pad(int left, int right) {
        coef.resize(coef.size() + left + right, 0);
        std::rotate(coef.rbegin(), coef.rbegin() + right, coef.rend());
    }

    /**
     * @brief Trim the polynomial by removing leading and trailing zeros.
     * @param left Number of leading zeros to remove.
     * @param right Number of trailing zeros to remove.
     */
    void Trim(int left, int right) {
        coef.erase(coef.begin(), coef.begin() + left);
        coef.erase(coef.end() - right, coef.end());
    }

    /**
     * @brief Append another polynomial to this polynomial.
     * @param other Polynomial to append.
     */
    void Append(const Poly<T>& other) {
        coef.insert(coef.end(), other.coef.begin(), other.coef.end());
    }

}; // end Poly


/**
 * @brief Divide two polynomials with remainders!
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
};


/**
 * @brief Perform Chien search on a polynomial.
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
 * @brief Find irreducible polynomials (primitive polynomials) in the specified Galois field.
 * @tparam T Type of elements in the Galois field.
 * @param[out] out Vector to store the found primitive polynomials.
 * @param fieldPower Power of the Galois field.
 * @param degree Degree of the primitive polynomials to be found.
 * @param maxNumPolys Maximum number of primitive polynomials to find.
 *
 * This function finds primitive polynomials with a specified degree in a Galois field.
 * A primitive polynomial is irreducible and generates all non-zero elements of the Galois field.
 */
template <typename T>
void FindPrimePolys(std::ostream& out, int fieldPower, int limit) {
    GaloisField<T> gf(fieldPower);
    T fieldCharacteristic = (static_cast<T>(1) << fieldPower) - 1;
    T fieldCharacteristicNext = (static_cast<T>(1) << (fieldPower + 1)) - 1;
    int primesFound = 0;

    for (T i = fieldCharacteristic + 2; (fieldCharacteristicNext == 0 ? i > 0 : i < fieldCharacteristicNext); i += 2) {
        unsigned int x = 2;
        bool conflict = false;

        for (unsigned int j = 1; j < fieldCharacteristic; j++) {
            x = (x << 1) > fieldCharacteristic ? x ^ i : x;
            if (x == 2) {
                conflict = true;
                break;
            }
        }
        if (!conflict) {
            out << std::hex << i << '\n';
            if (++primesFound >= limit)
                return;
        }
    }
}

