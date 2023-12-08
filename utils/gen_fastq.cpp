#include <iostream>
#include <fstream>
#include <vector>
#include <random>

// Function to generate a random quality score
char generate_random_quality() {
    static const std::string quality_chars = "ABCDEFGHIJ";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> distribution(0, quality_chars.size() - 1);

    return quality_chars[distribution(gen)];
}

void convert_to_fastq(const std::string& input_filename) {
    // Use the automatic type deduction feature (auto)
    std::ifstream input_file(input_filename);
    if (!input_file.is_open()) {
        std::cerr << "Error opening input file: " << input_filename << std::endl;
        return;
    }

    // Generate output filename by appending ".fastq" to the input filename
    const std::string output_filename = input_filename + ".fastq";
    std::ofstream output_file(output_filename);
    if (!output_file.is_open()) {
        std::cerr << "Error opening output file: " << output_filename << std::endl;
        return;
    }

    // Use range-based for loop
    for (std::string sequence; std::getline(input_file, sequence);) {
        // Use range-based for loop with auto
        std::string quality(sequence.size(), ' ');
        for (auto& q : quality) {
            q = generate_random_quality();
        }

        // Write the FASTQ entry
        output_file << "@\n";
        output_file << sequence << "\n";
        output_file << "+\n";
        output_file << quality << "\n";
    }

    std::cout << "FASTQ file created: " << output_filename << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_filename>" << std::endl;
        return 1;
    }

    // Use automatic type deduction for const
    const std::string input_filename = argv[1];

    convert_to_fastq(input_filename);

    return 0;
}

