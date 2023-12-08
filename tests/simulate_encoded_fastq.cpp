#include "../src/codec.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <stdexcept>

template <typename T>
void fisher_yates_shuffle(std::vector<T>& vec)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    for (size_t i = vec.size() - 1; i > 0; --i) {
        std::uniform_int_distribution<size_t> distribution(0, i);
        size_t j = distribution(gen);

        std::swap(vec[i], vec[j]);
    }
}

// Generate a random quality score
char generate_random_quality() {
    static const std::string quality_chars = "ABCDEFGHIJ";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> distribution(0, quality_chars.size() - 1);

    return quality_chars[distribution(gen)];
}

// Create sample fastq file
void convert_to_fastq(const std::vector<std::string>& sequences, const std::string& output_filename) {
    std::ofstream output_file(output_filename);
    if (!output_file.is_open()) {
        std::cerr << "Error opening output file: " << output_filename << std::endl;
        return;
    }

    for (const auto& sequence : sequences) {
        std::string quality(sequence.size(), ' ');
        for (auto& q : quality) {
            q = generate_random_quality();
        }

        output_file << "@\n";
        output_file << sequence << "\n";
        output_file << "+\n";
        output_file << quality << "\n";
    }

    std::cout << "FASTQ file created: " << output_filename << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const std::string input_filename = argv[1];

    Codec codec(input_filename);
    codec.print_info();
    codec.encode();

    std::vector<std::string> vec = codec.get_duplex_vec();

    fisher_yates_shuffle(vec);

    convert_to_fastq(vec, input_filename + ".fastq");

    return 0;
}

