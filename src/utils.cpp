#include "utils.hpp"


std::string nt2string(int nt) {
    if (nt >= 0 && nt < static_cast<int>(nucleotideStr.size())) {
        return { nucleotideStr[nt] };
    }
    else {
        return "?";
    }
}

/**
 * Using std::optional as the return type makes it clearer that this function might not find a corresponding value for the given character.
 * Using std::unordered_map to map characters to integer values provides a more concise representation of this mapping relationship.
 */
std::optional<int> char2nt(char nt) {
    static const std::unordered_map<char, int> nucleotideMap {
        {'A', static_cast<int>(Nucleotide::A)},
        {'T', static_cast<int>(Nucleotide::T)},
        {'C', static_cast<int>(Nucleotide::C)},
        {'G', static_cast<int>(Nucleotide::G)}
    };

    if (auto it = nucleotideMap.find(nt); it != nucleotideMap.end())
        return it->second;
    else
        return std::nullopt; // Indicates when the corresponding value is not found.
}

int string2nt(const std::string& nt) {
    if (nt.size() == 1) {
        auto result = char2nt(nt[0]);
        if (result.has_value()) {
            return result.value(); // Retrieve the value from std::optional<int>.
        }
    }
    return -1;
}

// Using a range-based for loop combined with std::views::filter view applies the filtering condition directly to the range.
double calculateGCContent(const std::string& sequence) {
    if (sequence.empty()) return 0.0;

    int gcCount = 0;
    for (char nucleotide : sequence | std::views::filter([](char nt) {
                return nt == 'C' || nt == 'G';
                })) {
        ++gcCount;
    }

    return static_cast<double>(gcCount) / sequence.length();
}

// Using iterators instead of a range-based for loop.
int calculateMaxHomopolymerLen(const std::string& sequence) {
    int maxhp = 0;
    int n = 1;

    char snt = (!sequence.empty()) ? sequence.front() : '\0';

    for (auto it = sequence.begin(); it != sequence.end(); ++it) {
        char nt = *it;
        n = (nt == snt) ? n + 1 : 1;
        maxhp = std::max(maxhp, n);
        snt = nt;
    }

    return maxhp;
}

/**
 * Replaced std::transform with std::ranges::transform
 * and std::reverse with std::ranges::reverse.
 * These functions handle ranges more directly.
 */
std::string revcom(const std::string& dna) {
    auto complement = [](char base) {
        switch (base) {
            case 'A': return 'T';
            case 'T': return 'A';
            case 'C': return 'G';
            case 'G': return 'C';
            default:  return base;
        }
    };

    std::string oligo = dna;
    std::ranges::transform(oligo, oligo.begin(), complement);
    std::ranges::reverse(oligo);

    return oligo;
}

/**
 * Using initializer lists for std::vector and range-based for loops introduced in C++20. Also leveraging some
 * newly introduced algorithm functions from the library like std::ranges::views::iota and std::ranges::for_each.
 */
int levenshtein_distance(const std::string& str1, const std::string& str2) {
    int len1 = str1.length();
    int len2 = str2.length();

    std::vector<std::vector<int>> matrix(len1 + 1, std::vector<int>(len2 + 1));

    std::ranges::for_each(std::ranges::views::iota(0, len1 + 1), [&](int i) {
            std::ranges::for_each(std::ranges::views::iota(0, len2 + 1), [&](int j) {
                    matrix[i][j] = (i == 0) ? j : ((j == 0) ? i : 0);
                    });
            });

    std::ranges::for_each(std::ranges::views::iota(1, len1 + 1), [&](int i) {
            std::ranges::for_each(std::ranges::views::iota(1, len2 + 1), [&](int j) {
                    int cost = (str1[i - 1] != str2[j - 1]);
                    matrix[i][j] = std::min({ matrix[i - 1][j] + 1, matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + cost });
                    });
            });

    return matrix[len1][len2];
}

/**
 *Employing range-based for loops, introducing std::transform, and utilizing lambda expressions.
 *Used for transforming and updating operations on sequences.
 */
bool Match(const std::string& p, const std::string& q, int maxdist) {
    std::vector<std::vector<int>> dp(p.size() + 1, std::vector<int>(q.size() + 1, 0));

    // Initialize the dynamic programming matrix:
    for (size_t i = 0; i <= p.size(); ++i) {
        for (size_t j = 0; j <= q.size(); ++j) {
            if (i == 0)
                dp[i][j] = j;
            else if (j == 0)
                dp[i][j] = i;
            else
                dp[i][j] = 0;
        }
    }

    //Fill in the dynamic programming matrix:
    for (size_t i = 1; i <= p.size(); ++i) {
        for (size_t j = 1; j <= q.size(); ++j) {
            int cost = (p[i - 1] == q[j - 1]) ? 0 : 1;
            dp[i][j] = std::min({ dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost });
        }
    }

    return dp[p.size()][q.size()] <= maxdist;
}

/**
 *Using std::min along with a custom comparison function, comparing two std::pair<int, int>.
 std::tie is used to create tuples for comparison, which compares individual members of the tuples.
 */
std::pair<int, int> min2(int a, int b, int aa, int bb) {
    return std::min(std::make_pair(a, aa), std::make_pair(b, bb), [](const auto& lhs, const auto& rhs) {
            return std::tie(lhs.first, lhs.second) < std::tie(rhs.first, rhs.second);
            });
}

// Simplifying using fold expressions.
void update_distances(int& fj1, int& lj1, int cbMismatch, int& mdist, int& mn, int& ln) {
    mn = std::min({ mn, fj1 + 1, ln, lj1 + cbMismatch });
    fj1 = ln;
    lj1 = mn;

    mdist = std::min({ mdist, ln }); 
}

// Utilizing std::min_element function to find the minimum distance, combined with some algorithm functions for a more concise code.
bool Find(const std::string& s, const std::string& subseq, int maxdist, int& pos, int& length) {
    int slen = s.length();
    int sslen = subseq.length();

    std::vector<std::vector<int>> distances(sslen + 1, std::vector<int>(slen + 1, 0));

    for (int n = 1; n <= sslen; ++n)
        distances[n][0] = n;

    for (int m = 1; m <= slen; ++m)
        distances[0][m] = m;

    for (int n = 1; n <= sslen; ++n) {
        for (int m = 1; m <= slen; ++m) {
            int cbMismatch = (subseq[n - 1] != s[m - 1]) ? 1 : 0;
            distances[n][m] = std::min({ distances[n - 1][m] + 1, distances[n][m - 1] + 1, distances[n - 1][m - 1] + cbMismatch });
        }
    }

    auto minDistanceIter = std::min_element(distances[sslen].begin(), distances[sslen].end());
    int minDistance = *minDistanceIter;
    int end = static_cast<int>(std::distance(distances[sslen].begin(), minDistanceIter));

    if (minDistance > maxdist) {
        pos = -1;
        length = 0;
        return false;
    }

    length = minDistance;
    pos = end - length;

    if (pos < 0) {
        length += pos;
        pos = 0;
    }

    return true;
}

// Using structured binding of std::pair to improve the readability of the returned value.
std::pair<int, int> FindPrefix(const std::string& s, const std::string& prefix, int maxdist) {
    int pos, length;
    bool found = Find(s, prefix, maxdist, pos, length);

    if (found) {
        return { pos, length };
    }
    else {
        return { -1, 0 };
    }
}

// Using structured binding of std::pair to improve the readability of the returned value.
std::pair<int, int> FindSuffix(const std::string& s, const std::string& suffix, int maxdist) {
    std::string reversedS(s.rbegin(), s.rend());
    std::string reversedSuffix(suffix.rbegin(), suffix.rend());

    int pos, length;
    bool found = Find(reversedS, reversedSuffix, maxdist, pos, length);

    if (found) {
        int reversedPos = s.length() - pos - length;
        return { reversedPos, length };
    }
    else {
        return { -1, 0 };
    }
}

/**
 *Using 'using' to alias types for improved clarity and readability.
 *The return value remains unchanged, still returning a std::pair<int, std::string>,
 *which includes the minimum edit distance and the string describing the edit operations.
 */
using Matrix = std::vector<std::vector<int>>;
using ActionMatrix = std::vector<std::vector<std::string>>;
std::pair<int, std::string> Diff(const std::string& from, const std::string& to) {
    int m = from.size();
    int n = to.size();

    // Initialize matrices
    Matrix v(m + 1, std::vector<int>(n + 1));
    ActionMatrix b(m + 1, std::vector<std::string>(n + 1, ""));
    // Initialize base cases
    for (int i = 1; i <= n; ++i) {
        v[0][i] = i;
        b[0][i] = "I";
    }

    for (int i = 1; i <= m; ++i) {
        v[i][0] = i;
        b[i][0] = "D";
    }

    // Dynamic programming to compute minimum edit distance
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int deletionCost = v[i - 1][j] + 1;
            int insertionCost = v[i][j - 1] + 1;
            int substitutionCost = v[i - 1][j - 1];

            if (from[i - 1] != to[j - 1])
                substitutionCost++;

            int mincost = std::min({ insertionCost, deletionCost, substitutionCost });

            v[i][j] = mincost;

            if (mincost == deletionCost)
                b[i][j] = "D";
            else if (mincost == insertionCost)
                b[i][j] = "I";
            else
                b[i][j] = (substitutionCost == mincost) ? "R" : "-";
        }
    }

    // Backtrack to get the edit actions
    std::string diff = "";
    for (int i = m, j = n; i > 0 || j > 0;) {
        switch (b[i][j][0]) {
            case 'D':
                diff = "D" + diff;
                i--;
                break;
            case 'R':
                diff = (from[i - 1] != to[j - 1]) ? "R" + diff : "-" + diff;
                i--;
                j--;
                break;
            case 'I':
                diff = "I" + diff;
                j--;
                break;
        }
    }

    return { v[m][n], diff };
}

