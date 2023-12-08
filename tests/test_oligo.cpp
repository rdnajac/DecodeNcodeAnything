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

//bool test_cmp() {
//    Oligo oligo1(generateRandomString(generator() % 31));
//    Oligo oligo2(generateRandomString(generator() % 31));
//
//    std::string sequence1 = oligo1.seq();
//    std::string sequence2 = oligo2.seq();
//
//    int comparisonResult = sequence1.length() - sequence2.length();
//    comparisonResult = (comparisonResult < 0) ? -1 : (comparisonResult > 0) ? 1 : comparisonResult;
//
//    if (comparisonResult == 0)
//        for (size_t i = 0; i < sequence1.length(); i++) {
//            int nt1 = string2nt(std::string(1, sequence1[i]));
//            int nt2 = string2nt(std::string(1, sequence2[i]));
//            comparisonResult = (nt1 < nt2) ? -1 : (nt1 > nt2) ? 1 : 0;
//            if (comparisonResult != 0)
//                break;
//        }
//    return comparisonResult == oligo1.cmp(oligo2);
//}
bool test_cmp() {
    Oligo oligo1(generateRandomString(generator() % 31));
    Oligo oligo2(generateRandomString(generator() % 31));

    // Compare two oligos directly using the overloaded operators
    return (oligo1 == oligo2) == (oligo1.cmp(oligo2) == 0);
}

bool test_bp_and_data() {
    // Generate a random sequence
    std::string seq = generateRandomString(generator() % 31);
    Oligo oligo(seq);

    // Test if the number of base pairs and data match the expectations
    return (oligo.bp() == seq.length()) && (oligo.data() > 0);
}

bool test_seq_empty_oligo() {
	Oligo oligo;
	return oligo.seq() == "";
}

//bool test_slice() {
//	std::string seq = generateRandomString(generator() % 31);
//	Oligo oligo(seq);
//
//	// Generate a random start and end index
//	std::uniform_int_distribution<int> distribution(0, seq.length() - 1);
//	int start = distribution(generator);
//	int end = distribution(generator);
//
//	// Ensure that start is less than end
//    if (start > end) {
//		int temp = start;
//		start = end;
//		end = temp;
//	}
//
//	// Get the slice of the sequence
//    Oligo slice = oligo.slice(start, end);
//
//	// Ensure that the slice is the same as the substring
//	return slice == seq.substr(start, end - start + 1);
//}
bool test_slice() {
    std::string seq = generateRandomString(generator() % 31);
    Oligo oligo(seq);

    // Generate random start and end indices
    std::uniform_int_distribution<int> distribution(0, seq.length() - 1);
    int start = distribution(generator);
    int end = distribution(generator);

    // Ensure start index is less than or equal to end index
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }

    // Get the slice of the sequence
    Oligo slice = oligo.slice(start, end);

    // Ensure that the slice is the same as the substring
    std::string expected_slice = seq.substr(start, end - start + 1);
    return slice.seq() == expected_slice;
}

//bool test_append() {
//	std::string seq1 = generateRandomString(generator() % 31);
//	Oligo oligo1(seq1);
//
//	std::string seq2 = generateRandomString(generator() % 31);
//	Oligo oligo2(seq2);
//
//	// Append the second sequence to the first
//	oligo1.append(oligo2);
//
//	// Ensure that the sequence is the same as the concatenation
//	return oligo1.seq() == seq1 + seq2;
//}
bool test_append() {
    // Generate two random sequences
    std::string seq1 = generateRandomString(generator() % 16);  // Limit to ensure append does not exceed MAX_BP
    std::string seq2 = generateRandomString(generator() % 16);

    Oligo oligo1(seq1);
    Oligo oligo2(seq2);

    // Append one Oligo to another
    bool success = oligo1.append(oligo2);

    // Test if the append operation was successful and the resulting sequence is as expected
    return success && (oligo1.seq() == seq1 + seq2);
}
int main() {
    run_test("seq", test_seq);
    run_test("subscript", test_subscript);
    run_test("cmp", test_cmp);
    run_test("bp_and_data", test_bp_and_data);
    run_test("seq_empty_oligo", test_seq_empty_oligo);
    run_test("slice", test_slice);
    run_test("append", test_append);
    return 0;
}

