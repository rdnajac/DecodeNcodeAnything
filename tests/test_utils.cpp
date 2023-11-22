/* @file test.cp
 * @brief Harness for unit testing
 */
#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>
#include "utils.hpp"

void test_revcom(const std::vector<std::pair<std::string, std::string>>& test_cases);
void test_lev_distance(const std::vector<std::tuple<std::string, std::string, int>>& test_cases);
void test_match_function(const std::vector<std::tuple<std::string, std::string, int, bool>>& test_cases);
void test_find_function(const std::vector<std::tuple<std::string, std::string, int, int, int, bool>>& test_cases);

int main()
{
    // Test cases for revcom
    std::vector<std::pair<std::string, std::string>> revcom_test_cases = {
        {"ATCG", "CGAT"},
        {"GCTA", "TAGC"},
        {"AGCTCGATCG", "CGATCGAGCT"}
    };

    // Test cases for levenshtein_distance
    std::vector<std::tuple<std::string, std::string, int>> lev_distance_test_cases = {
        {"kitten", "sitting", 3},
        {"sunday", "saturday", 3},
        {"ACAAGTCGTAGGAAGCTAATAGGCGTTCACCT", "AGAAGTTGTGTACATCACTTAGGCGTTCACCT", 9},
        {"CGAATCGTATGGTACAGAATAGGCGTTCACCT", "GGAAGCGTCGGTTAGTCTTAGGCGTTCACCT", 9}
    };

    // Test cases for Match function
    std::vector<std::tuple<std::string, std::string, int, bool>> match_function_test_cases = {
        {"ACGT", "ACGT", 0, true},
        {"ACGT", "AGCT", 0, false},
        {"ACGT", "AGCT", 1, true},
        {"ACGTACGT", "ACGTGCGT", 1, true},
        {"ACGT", "TGCA", 3, true},
        {"ACGT", "XXXX", 2, false},
        {"ACGT", "AC?T", 0, true},
    };

    // Test cases for Find function
    std::vector<std::tuple<std::string, std::string, int, int, int, bool>> find_function_test_cases = {
        {"ACGTAGCTGATCG", "CTGA", 2, 2, 4, true},
        {"ACGTAGCTGATCG", "CTGA", 1, 4, 4, true},
        {"ACGTAGCTGATCG", "CTGA", 0, 8, 4, false},
        {"ACGTAGCTGATCG", "TGCA", 3, 7, 4, true},
    };


    // Run tests
    test_revcom(revcom_test_cases);
    test_lev_distance(lev_distance_test_cases);
    test_match_function(match_function_test_cases);
    test_find_function(find_function_test_cases);

    return 0;
}

void test_revcom(const std::vector<std::pair<std::string, std::string>>& test_cases) {
    // Run tests
    for (size_t i = 0; i < test_cases.size(); ++i) {
        const auto& test_case = test_cases[i];
        const std::string& input_dna = test_case.first;
        const std::string& expected_result = test_case.second;

        std::string result = revcom(input_dna);

        // Check results
        if (result != expected_result) {
            std::cerr << "Reverse Complement Test " << (i + 1) << " Failed: " << result << " != " << expected_result << std::endl;
        } else {
            std::cout << "Reverse Complement Test " << (i + 1) << " Passed: " << result << " == " << expected_result << std::endl;
        }
    }

    // Count and print the total number of tests passed
    int testsPassed = 0;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        const auto& test_case = test_cases[i];
        const std::string& input_dna = test_case.first;
        const std::string& expected_result = test_case.second;

        testsPassed += (revcom(input_dna) == expected_result);
    }

    std::cout << "Total Reverse Complement Tests Passed: " << testsPassed << " out of " << test_cases.size() << std::endl;
}

void test_lev_distance(const std::vector<std::tuple<std::string, std::string, int>>& test_cases) {
    // Run tests
    for (size_t i = 0; i < test_cases.size(); ++i) {
        const auto& test_case = test_cases[i];
        const std::string& str1 = std::get<0>(test_case);
        const std::string& str2 = std::get<1>(test_case);
        int expected_result = std::get<2>(test_case);

        int result = levenshtein_distance(str1, str2);

        // Check results
        if (result != expected_result) {
            std::cerr << "Levenshtein Distance Test " << (i + 1) << " Failed: " << result << " != " << expected_result << std::endl;
        } else {
            std::cout << "Levenshtein Distance Test " << (i + 1) << " Passed: " << result << " == " << expected_result << std::endl;
        }
    }

    // Count and print the total number of tests passed
    int testsPassed = 0;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        const auto& test_case = test_cases[i];
        const std::string& str1 = std::get<0>(test_case);
        const std::string& str2 = std::get<1>(test_case);
        int expected_result = std::get<2>(test_case);

        testsPassed += (levenshtein_distance(str1, str2) == expected_result);
    }

    std::cout << "Total Levenshtein Distance Tests Passed: " << testsPassed << " out of " << test_cases.size() << std::endl;
}

void test_match_function(const std::vector<std::tuple<std::string, std::string, int, bool>>& test_cases) {
    // Run tests
    for (size_t i = 0; i < test_cases.size(); ++i) {
        const auto& test_case = test_cases[i];
        const std::string& str1 = std::get<0>(test_case);
        const std::string& str2 = std::get<1>(test_case);
        int maxdist = std::get<2>(test_case);
        bool expected_result = std::get<3>(test_case);

        bool result = Match(str1, str2, maxdist);

        // Check results
        if (result != expected_result) {
            std::cerr << "Match Function Test " << (i + 1) << " Failed: " << result << " != " << expected_result << std::endl;
        } else {
            std::cout << "Match Function Test " << (i + 1) << " Passed: " << result << " == " << expected_result << std::endl;
        }
    }

    // Count and print the total number of tests passed
    int testsPassed = 0;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        const auto& test_case = test_cases[i];
        const std::string& str1 = std::get<0>(test_case);
        const std::string& str2 = std::get<1>(test_case);
        int maxdist = std::get<2>(test_case);
        bool expected_result = std::get<3>(test_case);

        testsPassed += (Match(str1, str2, maxdist) == expected_result);
    }

    std::cout << "Total Match Function Tests Passed: " << testsPassed << " out of " << test_cases.size() << std::endl;
}

void test_find_function(const std::vector<std::tuple<std::string, std::string, int, int, int, bool>>& test_cases) {
    // Initialize variable to count passed tests
    int testsPassed = 0;

    // Run tests
    for (size_t i = 0; i < test_cases.size(); ++i) {
        const auto& test_case = test_cases[i];
        const std::string& sequence = std::get<0>(test_case);
        const std::string& subsequence = std::get<1>(test_case);
        int maxdist = std::get<2>(test_case);
        int expected_pos = std::get<3>(test_case);
        int expected_length = std::get<4>(test_case);
        bool expected_result = std::get<5>(test_case);

        int pos, length;  // Declare pos and length here

        // Check results
        bool result = Find(sequence, subsequence, maxdist, pos, length);
        if (result == expected_result && pos == expected_pos && length == expected_length) {
            std::cout << "Find Function Test " << (i + 1) << " Passed: " << result
                      << ", Pos: " << pos << ", Length: " << length << std::endl;
            testsPassed++;
        } else {
            std::cerr << "Find Function Test " << (i + 1) << " Failed: " << result
                      << " != " << expected_result << ", Pos: " << pos << " != " << expected_pos << ", Length: " << length << " != " << expected_length << std::endl;
        }
    }

    // Print the total number of tests passed
    std::cout << "Total Find Function Tests Passed: " << testsPassed << " out of " << test_cases.size() << std::endl;
}

