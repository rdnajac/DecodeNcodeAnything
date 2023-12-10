#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <cmath>
#include <algorithm>

template <typename T>
concept UnsignedInteger = std::is_unsigned_v<T> &&
                         (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);

/**
 * @brief Class representing a Galois Field of order 2^fieldPower.
 * @tparam T Type of elements in the Galois Field.
 */
template <UnsignedInteger T>
class GaloisField {

private:
    const unsigned int fieldPower; ///< Power of the field (2^fieldPower).
    const T characteristic;       ///< Characteristic of the field (2^fieldPower).
    const T primitivePoly;        ///< Primitive polynomial for the field.
public:
    std::vector<T> powTable; ///< Table for exponentiation.
    std::vector<T> logTable; ///< Table for logarithms.

    /**
     * @brief Constructor for GaloisField.
     * @param fieldPower The power of the field (2^fieldPower).
     */
    GaloisField(unsigned int fieldPower)
        : fieldPower(fieldPower),
        characteristic(static_cast<T>(1) << fieldPower),
        primitivePoly(0),  // Replace with the actual primitive polynomial
        powTable(characteristic),
        logTable(characteristic) {
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
    ~GaloisField() = default;

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


template <UnsignedInteger T>
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
    size_t size() const { return coef.size(); }

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
        for (auto& coef : this->coef)
            coef = this->gf.mult(coef, scale);
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
/**
 * @brief Template class representing the Reed-Solomon error correction code.
 * @tparam T Type of elements in the Reed-Solomon code.
 */
template <UnsignedInteger T>class ReedSolomon {
    public:
        GaloisField<T> gf;  ///< Galois Field used for Reed-Solomon operations.

        /**
         * @brief Constructor for the ReedSolomon class.
         * @param fieldPower Power of the Galois Field.
         */
        ReedSolomon(int fieldPower) : gf(fieldPower) {}

        /**
         * @brief Generate the generator polynomial for Reed-Solomon codes.
         * @param out Output polynomial representing the generator.
         * @param nsym Number of symbols in the error-correction code.
         */
        void createGenerator(Poly<T>& out, int nsym) {
            out = Poly<T>(1, 0);
            out.coef[0] = 1;

            Poly<T> factor = Poly<T>(2, 0);
            factor.coef[0] = 1;

            for (int i = 0; i < nsym; i++) {
                factor.coef[1] = this->gf.powTable[i];
                out.Mult(factor, this->gf);
            }
        }

        /**
         * @brief Encode a message vector using Reed-Solomon error correction.
         *
         * This function encodes the input message vector by appending Reed-Solomon error correction codes.
         * It modifies the input vector in-place.
         *
         * @tparam T Type of elements in the vector.
         * @param data Vector containing the original message. The encoded message will be appended to this vector.
         * @param nsym Number of error correction symbols to add.
         * @return True if the encoding is successful, false otherwise.
         *
         * @note The input vector `data` will be modified to contain the encoded message.
         */
        bool encode(std::vector<T>& data, int nsym) {
            Poly<T> msg(data.begin(), data.end());
            Poly<T> generator, remainder;
            this->createGenerator(generator, nsym);
            msg.Pad(0, nsym);
            Poly_Div(remainder, msg, generator, this->gf);

            for (size_t i = 0; i < data.size(); ++i) {
                data[i] ^= (i < remainder.size()) ? remainder.coef[i] : 0;
            }

            return true; // Assuming success
        }

        /**
         * @brief Calculate syndromes for error detection.
         * @param out Output polynomial containing syndromes.
         * @param msg Input polynomial representing the received message.
         * @param nsym Number of symbols in the error-correction code.
         */
        void calcSyndromes(Poly<T>& out, const Poly<T>& msg, int nsym) {
            T temp[nsym + 1];

            for (int i = 0; i < nsym; i++)
                temp[nsym - i - 1] = msg.Eval(this->gf.powTable[i], this->gf);

            temp[nsym] = 0;  // pad

            out = Poly<T>(nsym + 1, temp);
        }

        /**
         * @brief Check if syndromes indicate errors in the received message.
         * @param synd Input polynomial containing syndromes.
         * @return True if errors are detected, false otherwise.
         */
        bool checkSyndromes(const Poly<T>& synd) {
            return std::all_of(synd.coef.begin(), synd.coef.end(), [](T value) { return value == 0; });
        }

        /**
         * @brief Find the errata locator polynomial based on error positions.
         * @param out Output errata locator polynomial.
         * @param errPos Positions of errors in the received message.
         * @return True if successful, false otherwise.
         */
        bool findErrataLocator(Poly<T>& out, const std::vector<unsigned int>& errPos) {
            out.setCopy(1, nullptr);
            out.coef[0] = 1;
            Poly<T> factor(2, nullptr);
            factor.coef[1] = 1;

            for (unsigned int i : errPos) {
                factor.coef[0] = this->gf.powTable[i];
                Poly_Mult(&out, &out, &factor, &this->gf);
            }

            return true;
        }

        /**
         * @brief Find the error evaluator polynomial.
         * @param synd Syndrome polynomial.
         * @param errLoc Error locator polynomial.
         * @param nsym Number of symbols in the codeword.
         * @return Resultant error evaluator polynomial.
         */
        Poly<T> findErrorEvaluator(const Poly<T>& synd, const Poly<T>& errLoc, int nsym) {
            Poly<T> out = synd;
            out.Mult(errLoc, this->gf);
            out.Trim(out.size() - nsym, 0);
            return out;
        }

        /**
         * @brief Correct errors in the received message based on syndromes and error positions.
         * @param msg Received message polynomial.
         * @param synd Syndromes of the received message.
         * @param errPos Positions of errors in the received message.
         * @return True if successful, false otherwise.
         */
        bool correctErrata(Poly<T>& msg, const Poly<T>& synd, const std::vector<unsigned int>& errPos) {
            std::vector<unsigned int> coefPos(0);
            for (unsigned int i : errPos)
                coefPos.push_back(msg.coef.size() - 1 - i);

            Poly<T> errLoc, errEval;
            this->findErrataLocator(errLoc, coefPos);
            this->findErrorEvaluator(errEval, synd, errLoc, errLoc.coef.size());

            std::vector<T> x(coefPos.size());
            for (int i = 0; i < x.size(); i++)
                x[i] = this->gf.powTable[coefPos[i]];

            Poly<T> e(msg.coef.size(), 0);
            for (int i = 0; i < x.size(); i++) {
                T xi = this->gf.powTable[this->gf.characteristic - coefPos[i]];
                T errLocPrime = 1;

                for (int j = 0; j < x.size(); j++)
                    if (j != i)
                        errLocPrime = this->gf.mult(errLocPrime, this->gf.add(1, this->gf.mult(xi, x[j])));

                if (errLocPrime == 0)
                    return false;

                T y = this->gf.mult(x[i], Poly_Eval(errEval, xi, this->gf));
                e.coef[errPos[i]] = this->gf.div(y, errLocPrime);
            }

            Add(msg, msg, e);
            return true;
        }

        /**
         * @brief Find the error locator polynomial.
         * @param out Output polynomial representing the error locator.
         * @param synd Input polynomial containing syndromes.
         * @param nsym Number of symbols in the error-correction code.
         * @param eraseLoc Array of erasure positions.
         * @param eraseCount Number of erasures.
         * @return True if the error locator is found, false otherwise.
         */
        bool findErrorLocator(Poly<T>& out, const Poly<T>& synd, int nsym, const Poly<T>* eraseLoc, int eraseCount) {
            Poly<T> errLoc(1, 1);
            Poly<T> oldLoc(1, 1);
            Poly<T> temp;

            if (eraseLoc) {
                errLoc = *eraseLoc;
                oldLoc = *eraseLoc;
            }

            int syndShift = 0;
            for (int i = nsym - eraseCount - 1; i >= 0; i--) {
                int K = i + syndShift + eraseCount;
                T delta = synd.coef[K];

                for (size_t j = 1; j < errLoc.size(); j++)
                    delta ^= this->gf.mult(errLoc.coef[errLoc.size() - j - 1], synd.coef[K + j]);

                oldLoc.Pad(0, 1);
                if (delta != 0) {
                    if (oldLoc.size() > errLoc.size()) {
                        temp = oldLoc;
                        temp.Scale(delta, this->gf);
                        errLoc = temp.Scale(this->gf.inv(delta), this->gf);
                    } else {
                        temp = oldLoc;
                        temp.Scale(delta, this->gf);
                        errLoc.Add(temp);
                    }
                }
            }

            int leading = 0;
            for (; errLoc.coef[leading] == 0; leading++);
            errLoc.Trim(leading, 0);

            int errs = errLoc.size() - 1;
            out = errLoc;
            if (errs * 2 - eraseCount > nsym)
                return false;

            return true;
        }

        /**
         * @brief Find the positions of errors in the received message.
         * @param out Output vector containing error positions.
         * @param errLoc Input polynomial representing the error locator.
         * @param n Number of symbols in the error-correction code.
         * @return True if errors are found, false otherwise.
         */
        bool findErrors(std::vector<unsigned int>& out, const Poly<T> errLoc, int n) {
            size_t errs = errLoc.size() - 1;
            Poly<T> linearEq;

            if (errLoc.size() == 1) {
                // Do something special here? idk
            } else if (errLoc.size() == 2) { // Linear equation
                linearEq = errLoc;
                out.push_back(this->gf.logTable[this->gf.div(linearEq.coef[0], linearEq.coef[1])]);
            } else {
                errLoc.reverse();
                Poly_ChienSearch(out, errLoc, n, this->gf);
            }

            if (out.size() != errs)
                return false;

            // Map to string pos
            for (auto& pos : out) {
                if (pos >= n) // Clearly something messed up
                    return false;

                pos = n - pos - 1;
            }

            return true;
        }

        /**
         * @brief Forney algorithm to calculate syndromes at error positions.
         * @param synd Input polynomial containing syndromes.
         * @param pos Vector of error positions.
         * @param n Number of symbols in the error-correction code.
         * @return Output polynomial containing syndromes at error positions.
         */
        Poly<T> forneySyndromes(const Poly<T>& synd, const std::vector<unsigned int>& pos, int n) {
            Poly<T> fsynd = synd;
            if (!pos.empty()) {
                for (unsigned int i : pos) {
                    T rev = static_cast<T>(n) - i - 1;
                    T x = this->gf.powTable[rev];

                    for (int j = fsynd.size() - 2; j >= 0; j--)
                        fsynd[j + 1] = this->gf.mult(fsynd[j + 1], x) ^ fsynd[j];
                }
            }
        }

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
        bool decode(T* wholeOut, T* out, T* data, int k, int nsym, std::vector<unsigned int> erasePos, bool debug) {
            Poly<T> synd;
            Poly<T> msg(k + nsym, data);

            if (!erasePos.empty()) {
                if (erasePos.size() > static_cast<size_t>(nsym)) {
                    if (debug) std::cout << "Too many erasures to be corrected" << std::endl;
                    return false;
                } else {
                    for (unsigned int i : erasePos)
                        msg.coef[i] = 0;
                }
            }

            this->calcSyndromes(synd, msg, nsym);

            if (!this->checkSyndromes(synd)) {
                if (debug) std::cout << "Errors detected, locating" << std::endl;
                Poly<T> fsynd, errLoc;
                this->forneySyndromes(fsynd, synd, erasePos, k + nsym);
                bool canLocate = this->findErrorLocator(errLoc, fsynd, nsym, nullptr, erasePos.size());

                if (!canLocate) {
                    if (debug) std::cout << "Too many errors to locate!" << std::endl;
                    return false;
                }

                std::vector<unsigned int> pos;
                canLocate = this->findErrors(pos, errLoc, k + nsym);

                if (!canLocate || !(pos.size() || !erasePos.empty())) {
                    if (debug) std::cout << "Errors unable to be located!" << std::endl;
                    return false;
                }

                if (debug) {
                    if (!pos.empty()) {
                        std::cout << "Additional errors detected at ";
                        for (unsigned int e : pos) {
                            std::cout << static_cast<int>(e) << ", ";
                        }
                    }

                    std::cout << "Correcting" << std::endl;
                }

                if (!erasePos.empty())
                    pos.insert(pos.begin(), erasePos.begin(), erasePos.end());

                bool success = this->correctErrata(msg, synd, pos);

                if (!success) {
                    if (debug) std::cout << "Decode failure!" << std::endl;
                    return false;
                }

                if (debug) std::cout << "Errors corrected" << std::endl;
            }

            if (wholeOut)
                std::copy(msg.data(), msg.data() + k + nsym, wholeOut);

            if (out)
                std::copy(msg.data(), msg.data() + k, out);

            return true;
        }

};

