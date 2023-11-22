#include "oligo.hpp"
#include "utils.hpp"
#include <algorithm>

Oligo::Oligo() : length(0), data_block(0) {}

Oligo::Oligo(size_t len, uint64_t val) : length(len), data_block(val) {}

Oligo::Oligo(const std::string& s) {
    data_block = 0;

    if (s.length() > 32) {
        length = 0;
        return;
    }
    length = s.length();

    for (char c : s)
        data_block = (data_block << 2) | char2nt(c);
}

Oligo::~Oligo() {}

size_t Oligo::len() const {
    return length;
}

uint64_t Oligo::getDataBlock() const {
    return data_block;
}

Oligo* Oligo::newOligo(size_t olen) {
    return (olen > 32) ? nullptr : new Oligo(olen, 0);
}

Oligo* Oligo::val(size_t olen, uint64_t val) {
    return (olen > 32) ? nullptr : new Oligo(olen, val);
}

std::pair<Oligo*, bool> Oligo::copy(const Oligo& o) {
    uint64_t v = 0;
    if (o.len() > 32) { return {nullptr, false}; }
    for (size_t i = 0; i < o.len(); ++i) { v = (v << 2) | static_cast<uint64_t>(o[i]); }
    return {val(o.len(), v), true};
}

int Oligo::cmp(const Oligo& other) const {
    if (len() < other.len()) { return -1; } else if (len() > other.len()) { return 1; }
    if (data_block < other.getDataBlock()) { return -1; } else if (data_block > other.getDataBlock()) { return 1; }
    return 0;
}

bool Oligo::operator==(const Oligo& other) const { return cmp(other) == 0; }
bool Oligo::operator<(const Oligo& other) const { return cmp(other) < 0; }
bool Oligo::operator>(const Oligo& other) const { return cmp(other) > 0; }
bool Oligo::operator<=(const Oligo& other) const { return cmp(other) <= 0; }
bool Oligo::operator>=(const Oligo& other) const { return cmp(other) >= 0; }
bool Oligo::operator!=(const Oligo& other) const { return cmp(other) != 0; }

Oligo& Oligo::operator++() {
    uint64_t max = (1ULL << (2 * len())) - 1;
    if (len() != 32 && data_block != max) { ++data_block; }
    return *this;
}

Oligo Oligo::operator++(int) {
    Oligo tmp(*this);
    operator++();
    return tmp;
}

int Oligo::operator[](size_t idx) const {
    return static_cast<int>((data_block >> (2 * (len() - idx - 1))) & 0x3);
}

Oligo* Oligo::slice(int start, int end) {
    if (end <= 0) { end = len() - end; }
    if (end > len()) { end = len(); } else if (end < 0) { end = 0; }
    if (start < 0 || start > len() || start > end) { return nullptr; }
    int olen = end - start;
    uint64_t omask = (1ULL << (2 * olen)) - 1;
    return val(olen, (data_block >> (2 * (len() - end))) & omask);
}

Oligo* Oligo::clone() {
    return new Oligo(len(), data_block);
}

bool Oligo::append(const Oligo& other) {
    if (len() + other.len() > 32) { return false; }
    for (size_t i = 0; i < other.len(); ++i) { int nt = other[i]; data_block = (data_block << 2) | static_cast<uint64_t>(nt); }
    return true;
}

uint64_t Oligo::uint64() {
    return data_block;
}

std::string Oligo::seq() const {
    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        int nt = static_cast<int>((data_block >> (2 * (length - i - 1))) & 0x3);
        result += nt2string(nt);
    }

    return result;
}

