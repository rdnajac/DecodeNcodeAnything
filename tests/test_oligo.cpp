#include <random>
#include <iostream>
#include "../src/oligo.cpp"
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

int main() {
    run_test("seq", test_seq);
    run_test("subscript", test_subscript);
    run_test("cmp", test_cmp);
    return 0;
}

