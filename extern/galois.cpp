#ifndef GALOIS_HPP
#define GALOIS_HPP

#include <cassert>
#include <cmath>
#include "poly.cpp"

template <typename T>
concept UnsignedInteger = std::is_unsigned_v<T> &&
                         (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);

/**
 * @brief Class representing a Galois Field of order 2^fieldPower.
 * @tparam T Type of elements in the Galois Field.
 */
template <typename T>
class GaloisField {
private:
    const unsigned int fieldPower;   ///< Power of the field (2^fieldPower).
    const T characteristic; ///< Characteristic of the field (2^fieldPower).
    const T primitivePoly;  ///< Primitive polynomial for the field.
                            ///
public:
    T* powTable;            ///< Table for exponentiation.
    T* logTable;            ///< Table for logarithms.
                            ///
    /**
     * @brief Constructor for GaloisField.
     * @param fieldPower The power of the field (2^fieldPower).
     */
    GaloisField(int fieldPower)
        : fieldPower(fieldPower),
          characteristic(static_cast<T>(1) << fieldPower),
          primitivePoly(0),  // Replace with the actual primitive polynomial
          powTable(new T[characteristic]),
          logTable(new T[characteristic]) {
        // Initialize powTable and logTable
        T x = 1;
        for (T i = 0; i < characteristic - 1; ++i) {
            powTable[i] = x;
            logTable[x] = i;
            x = multNoLUT(x, 2);
        }
    }

    /**
     * @brief Destructor for GaloisField.
     */
    ~GaloisField() {
        delete[] powTable;
        delete[] logTable;
    }

    /**
     * @brief Multiply two elements in the Galois Field without using lookup tables.
     * @param a First element.
     * @param b Second element.
     * @return Result of the multiplication.
     */
    T multNoLUT(T a, T b) {
        T result = 0;
        while (b > 0) {
            if (b & 1)
                result ^= a;
            a = multNoLUT(a, 2);
            b >>= 1;
        }
        return result;
    }

    /**
     * @brief Multiply two elements in the Galois Field using lookup tables.
     * @param a First element.
     * @param b Second element.
     * @return Result of the multiplication.
     */
    T mult(T a, T b) {
        if (a == 0 || b == 0)
            return 0;

        return powTable[(logTable[a] + logTable[b]) % (characteristic - 1)];
    }

    /**
     * @brief Divide two elements in the Galois Field.
     * @param a Numerator.
     * @param b Denominator (non-zero).
     * @return Result of the division.
     */
    T div(T a, T b) {
        assert(b != 0);
        if (a == 0)
            return 0;
        return powTable[(logTable[a] + (characteristic - 1) - logTable[b]) % (characteristic - 1)];
    }

    /**
     * @brief Raise an element to a power in the Galois Field.
     * @param x Base element.
     * @param power Exponent.
     * @return Result of the exponentiation.
     */
    T pow(T x, T power) {
        assert(x != 0 || power != 0);  // 0^0 is undefined
        if (x == 0)
            return 0;
        if (power == 0)
            return 1;
        return powTable[(logTable[x] * power) % (characteristic - 1)];
    }

    /**
     * @brief Compute the multiplicative inverse of an element in the Galois Field.
     * @param x Element to invert (non-zero).
     * @return Multiplicative inverse of x.
     */
    T inv(T x) {
        assert(x != 0);
        return powTable[(characteristic - 1) - logTable[x]];
    }

    /**
     * @brief Compute the square root of an element in the Galois Field.
     * @param x Element for which to compute the square root (non-zero).
     * @return Square root of x.
     */
    T sqrt(T x) {
        assert(x != 0);
        return powTable[(logTable[x] + ((characteristic - 1) - 1)) / 2];
    }
};

#endif
