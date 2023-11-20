/* @file utils.cpp
 * @brief A collection of helper functions
 */

#include <string>
#include <vector>
#include <algorithm>

int levenshtein_distance(const std::string &str1, const std::string &str2)
{
	int len1 = str1.length();
	int len2 = str2.length();

	// Create a matrix
	std::vector<std::vector<int>> matrix(len1 + 1, std::vector<int>(len2 + 1));

	// Initialize the matrix
	for (int i = 0; i <= len1; ++i) {
		for (int j = 0; j <= len2; ++j) {
			if (i == 0)
				matrix[i][j] = j;
			else if (j == 0)
				matrix[i][j] = i;
			else
				matrix[i][j] = 0; // Placeholder, to be filled in the next loop
		}
	}

	// Fill  the matrix
	for (int i = 1; i <= len1; ++i) {
		for (int j = 1; j <= len2; ++j) {
			int cost = (str1[i - 1] != str2[j - 1]);		// 0 if chars match, 1 otherwise
			matrix[i][j] = std::min({matrix[i - 1][j] + 1,		// Deletion
					 	matrix[i][j - 1] + 1,		// Insertion
				 		matrix[i - 1][j - 1] + cost});	// Substitution
		}
	}

	return matrix[len1][len2];
}

