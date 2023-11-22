#include "utils.hpp"

std::string nt2string(int nt) {
    return (nt >= 0 && nt < nucleotideStr.size()) ? std::string(1, nucleotideStr[nt]) : "?";
}

int char2nt(char nt) {
    switch (nt) {
        case 'A': return static_cast<int>(Nucleotide::A);
        case 'T': return static_cast<int>(Nucleotide::T);
        case 'C': return static_cast<int>(Nucleotide::C);
        case 'G': return static_cast<int>(Nucleotide::G);
        default: return -1;
    }
}

int string2nt(const std::string& nt) {
    return (nt.size() == 1) ? char2nt(nt[0]) : -1;
}

double calculateGCContent(const std::string& sequence) {
    int n = 0;

    for (char nucleotide : sequence)
        if (nucleotide == 'C' || nucleotide == 'G')
            n++;

    return (sequence.length() > 0) ? static_cast<double>(n) / sequence.length() : 0.0;
}

int calculateMaxHomopolymerLen(const std::string& sequence) {
    int maxhp = 0;
    int n = 1;

    char snt = (sequence.length() > 0) ? sequence[0] : '\0';

    for (char nt : sequence) {
        if (nt == snt) {
            n++;
        } else {
            maxhp = std::max(maxhp, n);
            snt = nt;
            n = 1;
        }
    }

    return std::max(maxhp, n);
}

int levenshtein_distance(const std::string& str1, const std::string& str2) {
    int len1 = str1.length();
    int len2 = str2.length();

    std::vector<std::vector<int>> matrix(len1 + 1, std::vector<int>(len2 + 1));

    for (int i = 0; i <= len1; ++i) {
        for (int j = 0; j <= len2; ++j) {
            if (i == 0)
                matrix[i][j] = j;
            else if (j == 0)
                matrix[i][j] = i;
            else
                matrix[i][j] = 0;
        }
    }

    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            int cost = (str1[i - 1] != str2[j - 1]);
            matrix[i][j] = std::min({matrix[i - 1][j] + 1, matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + cost});
        }
    }

    return matrix[len1][len2];
}

bool Match(const std::string& p, const std::string& q, int maxdist) {
    std::vector<int> f(p.size() + 1, 0);

    for (int n = 0; n < q.size(); ++n) {
        char cq = q[n];
        int fj1 = f[0];
        f[0]++;
        for (int m = 0; m < p.size(); ++m) {
            int cb = (p[m] == '?' || p[m] == cq) ? 0 : 1;
            int mn = std::min({f[m + 1] + 1, f[m] + 1, fj1 + cb});

            fj1 = f[m + 1];
            f[m + 1] = mn;
        }
    }

    return f.back() <= maxdist;
}

std::pair<int, int> min2(int a, int b, int aa, int bb) {
    return (a <= b) ? std::make_pair(a, aa) : std::make_pair(b, bb);
}

void updateDistances(int& fj1, int& lj1, int cbMismatch, int& mdist, int& mn, int& ln) {
    mn = std::min({mn, fj1 + 1, ln, lj1 + cbMismatch});
    fj1 = ln;
    lj1 = mn;

    if (ln < mdist)
        mdist = ln;
}

bool Find(const std::string& s, const std::string& subseq, int maxdist, int& pos, int& length) {
    int slen = s.length();
    int sslen = subseq.length();

    std::vector<int> f(slen + 1, 0);
    std::vector<int> l(slen + 1, 0);

    for (int n = 0; n < sslen; ++n) {
        char cq = subseq[n];
        int fj1 = f[0];
        int lj1 = l[0];
        f[0]++;
        l[0]++;
        int mdist = f[0];

        for (int m = 0; m < slen; ++m) {
            char cb = s[m];

            int cbMismatch = (cb != cq) ? 1 : 0;
            auto [mn, ln] = min2(f[m + 1] + 1, f[m] + 1, l[m + 1] - 1, l[m] + 1); // delete & insert

            updateDistances(fj1, lj1, cbMismatch, mdist, mn, ln); // change or matched
            f[m + 1] = mn;
            l[m + 1] = ln;
        }

        if (mdist > maxdist) {
            pos = -1;
            length = 0;
            return false;
        }
    }

    int end = f.size() - 1;
    int minval = f[end];

    for (int i = end - 1; i >= 0; --i) {
        if (minval > f[i]) {
            minval = f[i];
            end = i;
        }
    }

    length = sslen + l[end];
    pos = end - length;

    if (pos < 0) {
        length += pos;
        pos = 0;
    }

    return true;
}

std::pair<int, int> FindPrefix(const std::string& s, const std::string& prefix, int maxdist) {
    int pos, length;
    if (Find(s, prefix, maxdist, pos, length)) {
        return {pos, length};
    } else {
        return {-1, 0};
    }
}

std::pair<int, int> FindSuffix(const std::string& s, const std::string& suffix, int maxdist) {
    std::string reversedS(s.rbegin(), s.rend());
    std::string reversedSuffix(suffix.rbegin(), suffix.rend());

    int pos, length;
    if (Find(reversedS, reversedSuffix, maxdist, pos, length)) {
        int reversedPos = s.length() - pos - length;
        return {reversedPos, length};
    } else {
        return {-1, 0};
    }
}

std::pair<int, std::string> Diff(const std::string& from, const std::string& to) {
    int m = from.size();
    int n = to.size();

    // Initialize matrices
    std::vector<std::vector<int>> v(m + 1, std::vector<int>(n + 1));
    std::vector<std::vector<std::string>> b(m + 1, std::vector<std::string>(n + 1));

    // Initialize base cases
    b[0][0] = "";
    v[0][0] = 0;

    for (int i = 1; i <= n; ++i) {
        v[0][i] = i;
        b[0][i] = "I";
    }

    // Dynamic programming to compute minimum edit distance
    for (int i = 0; i < m; ++i) {
        v[i + 1][0] = i + 1;
        b[i + 1][0] = "D";

        for (int j = 0; j < n; ++j) {
            int deletionCost = v[i][j + 1] + 1;
            int insertionCost = v[i + 1][j] + 1;
            int substitutionCost = v[i][j];

            b[i + 1][j + 1] = "R";

            if (from[i] != to[j]) {
                substitutionCost++;
            }

            int mincost = substitutionCost;

            if (mincost > insertionCost) {
                mincost = insertionCost;
                b[i + 1][j + 1] = "I";
            }

            if (mincost > deletionCost) {
                mincost = deletionCost;
                b[i + 1][j + 1] = "D";
            }

            v[i + 1][j + 1] = mincost;
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
                if (i > 0 && j > 0 && v[i - 1][j - 1] != v[i][j]) {
                    diff = "R" + diff;
                } else {
                    diff = "-" + diff;
                }
                i--;
                j--;
                break;

            case 'I':
                diff = "I" + diff;
                j--;
                break;
        }
    }

    return {v[m][n], diff};
}

