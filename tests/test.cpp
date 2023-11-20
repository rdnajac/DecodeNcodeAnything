/* @file test.cp
 * @brief Harness for unit testing
 */
#include <iostream>
#include <cassert>

#include "../oligo/utils.cpp"

#include <iostream>
#include <cassert>


// Test function
void test_levenshtein_distance()
{
    // Test cases
    std::string test1_str1 = "kitten";
    std::string test1_str2 = "sitting";
    int test1_expected = 3;

    std::string test2_str1 = "sunday";
    std::string test2_str2 = "saturday";
    int test2_expected = 3;

    // Run tests
    int result1 = levenshtein_distance(test1_str1, test1_str2);
    int result2 = levenshtein_distance(test2_str1, test2_str2);

    // Check results
    assert(result1 == test1_expected);
    assert(result2 == test2_expected);

    // Print results
    std::cout << "Test 1 Passed: " << result1 << " == " << test1_expected << std::endl;
    std::cout << "Test 2 Passed: " << result2 << " == " << test2_expected << std::endl;
}

int main()
{
    test_levenshtein_distance();

    return 0;
}

