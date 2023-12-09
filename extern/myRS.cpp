#include <iostream>
#include <vector>
#include <iomanip>
#include "galois.cpp"

template <typename T>
concept UnsignedInteger = std::is_unsigned_v<T> &&
                         (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);
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
