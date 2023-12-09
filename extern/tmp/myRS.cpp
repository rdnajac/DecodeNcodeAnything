#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "myRS.hpp"
#include "poly.cpp"

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

template class ReedSolomon<uint8_t>;
template class ReedSolomon<uint16_t>;
template class ReedSolomon<uint32_t>;
template class ReedSolomon<uint64_t>;

template <typename T>
ReedSolomon<T>::ReedSolomon(int fieldPower) : gf(fieldPower) {}

template <typename T>
void ReedSolomon<T>::createGenerator(Poly<T>& out, int nsym) {
    out.setCopy(1, nullptr);
    out.coef[0] = 1;
    Poly<T> factor(2, nullptr);
    factor.coef[0] = 1;

    for (int i = 0; i < nsym; i++) {
        factor.coef[1] = this->gf.powTable[i];
        Poly_Mult(&out, &out, &factor, &this->gf);
    }
}

template <typename T>
void ReedSolomon<T>::encode(T* out, T* data, int k, int nsym) {
    Poly<T> msg(k, data);
    Poly<T> generator, remainder;
    this->createGenerator(generator, nsym);
    Poly_Pad(&msg, 0, nsym);
    Poly_Div(nullptr, nullptr, &remainder, &msg, &generator, &this->gf);
    memcpy(out, data, sizeof(T) * k);
    memcpy(out + k, remainder.coef, sizeof(T) * remainder.size());
}

template <typename T>
void ReedSolomon<T>::calcSyndromes(Poly<T>& out, const Poly<T>& msg, int nsym) {
    T* temp = (T*)malloc(sizeof(T) * (nsym + 1));

    for (int i = 0; i < nsym; i++)
        temp[nsym - i - 1] = Poly_Eval(&msg, this->gf.powTable[i], &this->gf);

    temp[nsym] = 0;  // pad
    out.setRef(nsym + 1, temp);
}

template <typename T>
bool ReedSolomon<T>::checkSyndromes(const Poly<T>& synd) {
    for (int i = 0; i < synd.size(); i++)
        if (synd.coef[i])
            return false;

    return true;
}

template <typename T>
void ReedSolomon<T>::findErrataLocator(Poly<T>& out, const std::vector<unsigned int>& errPos) {
    out.setCopy(1, nullptr);
    out.coef[0] = 1;
    Poly<T> factor(2, nullptr);
    factor.coef[1] = 1;

    for (unsigned int i : errPos) {
        factor.coef[0] = this->gf.powTable[i];
        Poly_Mult(&out, &out, &factor, &this->gf);
    }
}

template <typename T>
void ReedSolomon<T>::findErrorEvaluator(Poly<T>& out, const Poly<T>& synd, const Poly<T>& errLoc, int nsym) {
    Poly_Mult(&out, &synd, &errLoc, &this->gf);
    Poly_Trim(&out, out.size() - nsym, 0);
}

template <typename T>
bool ReedSolomon<T>::correctErrata(Poly<T>& msg, const Poly<T>& synd, const std::vector<unsigned int>& errPos) {
    std::vector<unsigned int> coefPos(0);
    for (unsigned int i : errPos)
        coefPos.push_back(msg.size() - 1 - i);

    Poly<T> errLoc, errEval;
    this->findErrataLocator(errLoc, coefPos);
    this->findErrorEvaluator(errEval, synd, errLoc, errLoc.size());

    std::vector<T> x(coefPos.size());
    for (int i = 0; i < x.size(); i++)
        x[i] = this->gf.powTable[coefPos[i]];

    Poly<T> e(msg.size(), nullptr);
    for (int i = 0; i < x.size(); i++) {
        T xi = this->gf.powTable[this->gf.characteristic - coefPos[i]];
        T errLocPrime = 1;

        for (int j = 0; j < x.size(); j++)
            if (j != i)
                errLocPrime = this->gf.mult(errLocPrime, 1 ^ this->gf.mult(xi, x[j]));

        if (errLocPrime == 0)
            return false;

        T y = this->gf.mult(x[i], Poly_Eval(&errEval, xi, &this->gf));
        e.coef[errPos[i]] = this->gf.div(y, errLocPrime);
    }

    Poly_Add(&msg, &msg, &e);
    return true;
}

template <typename T>
bool ReedSolomon<T>::findErrorLocator(Poly<T>& out, const Poly<T>& synd, int nsym, const Poly<T>* eraseLoc, int eraseCount) {
    T init = 1;
    Poly<T> errLoc(1, &init);
    Poly<T> oldLoc(1, &init);
    Poly<T> temp;

    if (eraseLoc) {
        errLoc.setCopy(eraseLoc->n, eraseLoc->coef);
        oldLoc.setCopy(eraseLoc->n, eraseLoc->coef);
    }

    int syndShift = 0;
    for (int i = nsym - eraseCount - 1; i >= 0; i--) {
        int K = i + syndShift + eraseCount;
        T delta = synd.coef[K];

        for (int j = 1; j < errLoc.size(); j++)
            delta ^= this->gf.mult(errLoc.coef[errLoc.size() - j - 1], synd.coef[K + j]);

        Poly_Pad(&oldLoc, 0, 1);
        if (delta != 0) {
            if (oldLoc.size() > errLoc.size()) {
                Poly_Scale(&temp, &oldLoc, delta, &this->gf);
                Poly_Scale(&oldLoc, &errLoc, this->gf.inv(delta), &this->gf);
                errLoc.setCopy(temp.size(), temp.coef);
            }
            Poly_Scale(&temp, &oldLoc, delta, &this->gf);
            Poly_Add(&errLoc, &errLoc, &temp);
        }
    }

    int leading = 0;
    for (; errLoc.coef[leading] == 0; leading++);
    Poly_Trim(&errLoc, leading, 0);

    int errs = errLoc.size() - 1;
    out.setCopy(errLoc.size(), errLoc.coef);
    if (errs * 2 - eraseCount > nsym)
        return false;

    return true;
}

template <typename T>
bool ReedSolomon<T>::findErrors(std::vector<unsigned int>& out, const Poly<T>& errLoc, int n) {
    int errs = errLoc.size() - 1;
    Poly<T> revErrLoc;
    Poly_Reverse(&revErrLoc, &errLoc);

    if (errLoc.size() == 1)
        // Do something special here? idk
    else if (errLoc.size() == 2) // Linear equation
        out.push_back(this->gf.logTable[this->gf.div(errLoc.coef[0], errLoc.coef[1])]);
    else
        Poly_ChienSearch(out, &revErrLoc, n, &this->gf);

    if (out.size() != errs)
        // Too many (or few) errors found by Chien Search for the errata locator polynomial!
        return false;

    // Map to string pos
    for (T i = 0; i < out.size(); i++) {
        if (out[i] >= n) // Clearly something messed up
            return false;

        out[i] = n - out[i] - 1;
    }

    return true;
}

template <typename T>
void ReedSolomon<T>::forneySyndromes(Poly<T>& out, const Poly<T>& synd, const std::vector<unsigned int>& pos, int n) {
    Poly<T> fsynd(synd.size() - 1, synd.data());

    if (pos.size()) {
        for (unsigned int i : pos) {
            T rev = static_cast<T>(n) - i - 1;
            T x = this->gf.powTable[rev];

            for (int j = fsynd.size() - 2; j >= 0; j--)
                fsynd[j + 1] = this->gf.mult(fsynd[j + 1], x) ^ fsynd[j];
        }
    }
    // fsynd[fsynd.size() - 1] = 0; // Uncomment if you want to set the last coefficient to 0
    setCopy(fsynd.size(), fsynd.data());
}


template <typename T>
bool ReedSolomon<T>::decode(T* wholeOut, T* out, T* data, int k, int nsym, std::vector<unsigned int>* erasePos, bool debug) {
    Poly<T> synd;
    Poly<T> msg(k + nsym, data);

    if (erasePos) {
        if (erasePos->size() > nsym) {
            if (debug) std::cout << "Too many erasures to be corrected" << std::endl;
            return false;
        } else {
            for (unsigned int i : *erasePos)
                msg.coef[i] = 0;
        }
    }

    this->calcSyndromes(synd, msg, nsym);

    if (!this->checkSyndromes(synd)) {
        if (debug) std::cout << "Errors detected, locating" << std::endl;
        Poly<T> fsynd, errLoc;
        this->forneySyndromes(fsynd, synd, erasePos, k + nsym);
        bool canLocate = this->findErrorLocator(errLoc, fsynd, nsym, nullptr, erasePos ? erasePos->size() : 0);

        if (!canLocate) {
            if (debug) std::cout << "Too many errors to locate!" << std::endl;
            return false;
        }

        std::vector<unsigned int> pos;
        canLocate = this->findErrors(pos, errLoc, k + nsym);

        if (!canLocate || !(pos.size() || (erasePos && erasePos->size()))) {
            if (debug) std::cout << "Errors unable to be located!" << std::endl;
            return false;
        }

        if (debug) {
            if (pos.size()) {
                std::cout << "Additional errors detected at ";
                for_each(pos.begin(), pos.end(), [](unsigned int e) { std::cout << static_cast<int>(e) << ", "; });
            }

            std::cout << "Correcting" << std::endl;
        }

        if (erasePos)
            pos.insert(pos.begin(), erasePos->begin(), erasePos->end());

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

