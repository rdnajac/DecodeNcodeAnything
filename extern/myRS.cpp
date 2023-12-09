#include <iomanip>
#include <algorithm>

#include "myRS.hpp"


template <typename T>
GaloisField<T>::GaloisField(int fieldPower) : fieldPower(fieldPower), characteristic(1 << fieldPower),
                                               primitivePoly(FindPrimePoly(fieldPower)) {
    // Initialization of lookup tables
    powTable = new T[2 * characteristic];
    logTable = powTable + characteristic;
    T x = 1;

    for (T i = 0; i < characteristic - 1; i++) {
        powTable[i] = x;
        logTable[x] = i;
        x = x << 1;
        if (x >= characteristic)
            x = (x ^ primitivePoly) & (characteristic - 1);
    }

    powTable[characteristic - 1] = 1; // Repeat the first entry for 2^m - 1
    logTable[0] = logTable[characteristic - 1];

    if (fieldPower > 1) {
        for (T i = characteristic; i < 2 * characteristic - 1; i++)
            powTable[i] = powTable[i - characteristic];
    }
}

template <typename T>
GaloisField<T>::~GaloisField() {
    delete[] powTable;
}

template <typename T>
T GaloisField<T>::multNoLUT(T a, T b) {
    T result = 0;

    for (int i = 0; i < fieldPower; i++) {
        if (b & 1)
            result ^= a;
        b >>= 1;
        a <<= 1;
        if (a & characteristic)
            a ^= primitivePoly;
    }

    return result;
}

template <typename T>
T GaloisField<T>::mult(T a, T b) {
    if (a == 0 || b == 0)
        return 0;
    return powTable[(logTable[a] + logTable[b]) % (characteristic - 1)];
}

template <typename T>
T GaloisField<T>::div(T a, T b) {
    if (b == 0)
        throw std::domain_error("Division by zero in Galois Field.");
    if (a == 0)
        return 0;
    return powTable[(logTable[a] + characteristic - logTable[b]) % (characteristic - 1)];
}

template <typename T>
T GaloisField<T>::pow(T x, T power) {
    if (power < 0)
        throw std::domain_error("Negative exponent in Galois Field pow.");

    if (x == 0) {
        if (power == 0)
            throw std::domain_error("0^0 is undefined in Galois Field pow.");
        return 0; // All powers of 0 are 0
    }

    if (power == 0)
        return 1; // All powers of x^0 are 1

    return powTable[(logTable[x] * power) % (characteristic - 1)];
}

template <typename T>
T GaloisField<T>::inv(T x) {
    if (x == 0)
        throw std::domain_error("Inverse of zero in Galois Field.");
    return powTable[characteristic - logTable[x] - 1];
}

template <typename T>
T GaloisField<T>::sqrt(T x) {
    return pow(x, (characteristic - 1) / 2);
}

template class GaloisField<uint8_t>;
template class GaloisField<uint16_t>;
template class GaloisField<uint32_t>;
template class GaloisField<uint64_t>;

template <typename T>
Poly<T> Poly_Create(int n, const T* coef) {
    Poly<T> poly;
    poly.coef.assign(coef, coef + n + 1);
    return poly;
}

template <typename T>
void Poly_Add(Poly<T>& out, const Poly<T>& a, const Poly<T>& b) {
    int degA = a.coef.size() - 1;
    int degB = b.coef.size() - 1;
    int degSum = std::max(degA, degB);
    out.coef.resize(degSum + 1, 0);

    for (int i = 0; i <= degA; i++)
        out.coef[i + (degSum - degA)] ^= a.coef[i];

    for (int i = 0; i <= degB; i++)
        out.coef[i + (degSum - degB)] ^= b.coef[i];
}

template <typename T>
void Poly_Scale(Poly<T>& out, const Poly<T>& in, T scale, const GaloisField<T>& gf) {
    out = in;
    for (T& coef : out.coef)
        coef = gf.mult(coef, scale);
}

template <typename T>
void Poly_Mult(Poly<T>& out, const Poly<T>& a, const Poly<T>& b, const GaloisField<T>& gf) {
    int degA = a.coef.size() - 1;
    int degB = b.coef.size() - 1;
    out.coef.resize(degA + degB + 1, 0);

    for (int i = 0; i <= degA; i++)
        for (int j = 0; j <= degB; j++)
            out.coef[i + j] ^= gf.mult(a.coef[i], b.coef[j]);
}

template <typename T>
void Poly_Div(Poly<T>& result, Poly<T>& quotient, Poly<T>& remainder, const Poly<T>& a, const Poly<T>& b,
              const GaloisField<T>& gf) {
    int degA = a.coef.size() - 1;
    int degB = b.coef.size() - 1;
    remainder = a;
    result.coef.clear();

    for (int i = degA - degB; i >= 0; i--) {
        T factor = gf.div(remainder.coef[degB + i], b.coef[degB]);
        result.coef.insert(result.coef.begin(), factor);
        Poly<T> term = Poly_Create(degB + i, &result.coef[0]);
        Poly_Scale(term, b, factor, gf);
        Poly_Add(remainder, remainder, term);
    }

    quotient = result;
}

template <typename T>
T Poly_Eval(const Poly<T>& poly, T x, const GaloisField<T>& gf) {
    T result = 0;
    T xPow = 1;

    for (T coef : poly.coef) {
        result ^= gf.mult(coef, xPow);
        xPow = gf.mult(xPow, x);
    }

    return result;
}

template <typename T>
void Poly_ChienSearch(std::vector<unsigned int>& out, const Poly<T>& poly, int max, const GaloisField<T>& gf) {
    out.clear();

    for (unsigned int i = 1; i < gf.characteristic && out.size() < max; i++) {
        if (Poly_Eval(poly, gf.pow(2, i), gf) == 0)
            out.push_back(i);
    }
}

template <typename T>
void Poly_Pad(Poly<T>& poly, int left, int right) {
    poly.coef.insert(poly.coef.begin(), left, 0);
    poly.coef.resize(poly.coef.size() + right, 0);
}

template <typename T>
void Poly_Trim(Poly<T>& poly, int left, int right) {
    poly.coef.erase(poly.coef.begin(), poly.coef.begin() + left);
    poly.coef.resize(poly.coef.size() - right);
}

template <typename T>
void Poly_Append(Poly<T>& out, const Poly<T>& a, const Poly<T>& b) {
    out.coef = a.coef;
    out.coef.insert(out.coef.end(), b.coef.begin(), b.coef.end());
}

template <typename T>
void Poly_Reverse(Poly<T>& out, const Poly<T>& in) {
    out.coef = in.coef;
    std::reverse(out.coef.begin(), out.coef.end());
}

template class Poly<uint8_t>;
template class Poly<uint16_t>;
template class Poly<uint32_t>;
template class Poly<uint64_t>;

template class ReedSolomon<uint8_t>;
template class ReedSolomon<uint16_t>;
template class ReedSolomon<uint32_t>;
template class ReedSolomon<uint64_t>;
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
    Poly_Pad(msg, 0, nsym);
    Poly_Div(remainder, msg, generator, this->gf);
    for (int i = 0; i < k; ++i)
        out[i] = data[i];
    for (size_t i = 0; i < remainder.size(); ++i)
        out[k + i] = remainder.coef[i];
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
