#include <random>
#include <iostream>
#include "oligo.hpp"
#include "utils.hpp"

const int iternum = 5;

std::mt19937 generator(std::random_device{}());

std::string generateRandomString(int length) {
    length = (length == 0) ? 1 : length;
    std::uniform_int_distribution<int> distribution(0, 3);
    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; i++)
        result += nt2string(distribution(generator));
    return result;
}

// TODO fix this
//Oligo* generateRandomOligo(int length) {
//    std::string sequence = generateRandomString(length);
//    return (generator() % 3 == 0) ? Oligo::newOligo(sequence.length()) : Oligo::val(sequence.length(), 0);
//}


template <typename Func>
void run_test(const std::string& test, const Func& func) {
    for (int i = 0; i < iternum; i++) {
        if (!func()) {
            std::cout << test << " failed :(" << std::endl;
            return;
        }
    }
    std::cout << test << " successful!" << std::endl;
}

bool test_seq() {
    std::string seq = generateRandomString(generator() % 31);
    Oligo oligo(seq);

    return seq == oligo.seq();
}

bool test_subscript() {
    std::string seq1 = generateRandomString(generator() % 31);
    Oligo oligo1(seq1);

    std::string seq2;
    for (size_t i = 0; i < seq1.length(); i++)
        seq2 += nt2string(oligo1[i]);

    return seq1 == seq2;
}

bool test_cmp() {
    Oligo oligo1(generateRandomString(generator() % 31));
    Oligo oligo2(generateRandomString(generator() % 31));

    std::string sequence1 = oligo1.seq();
    std::string sequence2 = oligo2.seq();

    int comparisonResult = sequence1.length() - sequence2.length();
    comparisonResult = (comparisonResult < 0) ? -1 : (comparisonResult > 0) ? 1 : comparisonResult;

    if (comparisonResult == 0)
        for (size_t i = 0; i < sequence1.length(); i++) {
            int nt1 = string2nt(std::string(1, sequence1[i]));
            int nt2 = string2nt(std::string(1, sequence2[i]));
            comparisonResult = (nt1 < nt2) ? -1 : (nt1 > nt2) ? 1 : 0;
            if (comparisonResult != 0)
                break;
        }
    return comparisonResult == oligo1.cmp(oligo2);
}

void test_next() {
    std::mt19937 generator(std::random_device{}());

    for (int i = 0; i < iternum; i++) {
        Oligo* oligo1 = generateRandomOligo(generator() % 31);
        Oligo* oligo2 = oligo1->clone();
        oligo2->operator++();

        if (oligo1->cmp(*oligo2) != -1) {
            std::cerr << "Next() fails: " << oligo1->seq() << ": " << oligo2->seq() << std::endl;
            delete oligo1;
            delete oligo2;
            return;
        }

        delete oligo1;
        delete oligo2;
    }
    std::cout << "next successful!" << std::endl;
}

void test_slice() {
    std::mt19937 generator(std::random_device{}());

    for (int i = 0; i < iternum;) {
        Oligo* oligo1 = generateRandomOligo(generator() % 31);
        if (oligo1->len() < 4)
            continue;

        int start = generator() % oligo1->len();
        int end = generator() % oligo1->len();

        if (end <= start)
            continue;

        std::string sequence1;
        sequence1.reserve(end - start);
        for (int n = start; n < end; n++)
            sequence1 += nt2string(oligo1->operator[](n));

        Oligo* oligo2 = oligo1->slice(start, end);
        std::string sequence2 = oligo2->seq();
        if (sequence1 != sequence2) {
            std::cerr << "Slice() fails: " << sequence1 << ": " << sequence2 << std::endl;
            delete oligo1;
            delete oligo2;
            return;
        }

        i++;
    }
}

void test_copy() {

    for (int i = 0; i < iternum; i++) {
        Oligo* oligo1 = generateRandomOligo(generator() % 31);
        std::pair<Oligo*, bool> result = Oligo::copy(*oligo1);
        Oligo* oligo2 = result.first;

        if (!result.second || oligo1->cmp(*oligo2) != 0) {
            std::cerr << "Copy() fails: " << oligo1->seq() << ": " << oligo2->seq() << std::endl;
            delete oligo1;
            delete oligo2;
            return;
        }

        delete oligo1;
        delete oligo2;
    }
}
void test_append() {
    std::mt19937 generator(std::random_device{}());

    for (int i = 0; i < iternum; i++) {
        Oligo* oligo1 = generateRandomOligo(generator() % 31);
        Oligo* oligo2 = generateRandomOligo(generator() % 31);

        std::string sequence1 = oligo1->seq() + oligo2->seq();
        bool success = oligo1->append(*oligo2);
        std::string sequence2 = oligo1->seq();

        if (!success || sequence1 != sequence2) {
            std::cerr << "Append() fails: " << sequence1 << ": " << sequence2 << std::endl;
            delete oligo1;
            delete oligo2;
            return;
        }
    }
}

int main() {
    run_test("seq", test_seq);
    run_test("subscript", test_subscript);
    run_test("cmp", test_cmp);
    test_next();
    test_slice();
    test_append();
    test_copy();

    return 0;
}

#if 0
bool test_next() {
    Oligo* oligo1 = generateRandomOligo(generator() % 31);
    Oligo* oligo2 = oligo1->clone();
    oligo2->operator++();

    int result = oligo1->cmp(*oligo2);

    delete oligo1;
    delete oligo2;

    return result == -1;
}

bool test_slice() {
    Oligo* oligo1 = generateRandomOligo(generator() % 31);
    if (oligo1->len() < 4)
        return false;

    int start = generator() % oligo1->len();
    int end = generator() % oligo1->len();

    if (end <= start)
        return false;

    std::string sequence1;
    sequence1.reserve(end - start);
    for (int n = start; n < end; n++)
        sequence1 += nt2string(oligo1->operator[](n));

    Oligo* oligo2 = oligo1->slice(start, end);
    std::string sequence2 = oligo2->seq();

    delete oligo1;
    delete oligo2;

    return sequence1 == sequence2;
}

bool test_copy() {
    Oligo* oligo1 = generateRandomOligo(generator() % 31);
    std::pair<Oligo*, bool> result = Oligo::copy(*oligo1);
    Oligo* oligo2 = result.first;

    bool success = result.second && oligo1->cmp(*oligo2) == 0;

    delete oligo1;
    delete oligo2;

    return success;
}

bool test_append() {
    Oligo* oligo1 = generateRandomOligo(generator() % 31);
    Oligo* oligo2 = generateRandomOligo(generator() % 31);

    std::string sequence1 = oligo1->seq() + oligo2->seq();
    bool success = oligo1->append(*oligo2);
    std::string sequence2 = oligo1->seq();

    delete oligo1;
    delete oligo2;

    return success && sequence1 == sequence2;
}

int main() {
    run_test("next", test_next);
    run_test("slice", test_slice);
    run_test("append", test_append);
    run_test("copy", test_copy);

    return 0;
}
#endif
