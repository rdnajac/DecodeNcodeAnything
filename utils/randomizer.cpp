#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <stdexcept>
#include <filesystem>

// Function to shuffle a vector without using rand
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

int main(int argc, char* argv[]) try
{
    if (argc != 2)
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl, exit(1);

    const std::string input_filename = argv[1];
    const std::string output_filename = input_filename + ".random";

    std::vector<std::string> lines;
    {
        std::ifstream input_file(input_filename);
        if (!input_file.is_open())
            throw std::runtime_error("Error opening file: " + input_filename);

        std::string line;
        while (std::getline(input_file, line))
            lines.push_back(line);
    }

    // Shuffle the vector
    fisher_yates_shuffle(lines);

    // Write shuffled lines back to the file
    {
        std::ofstream output_file(output_filename);
        if (!output_file.is_open())
            throw std::runtime_error("Error opening file for writing: " + output_filename);

        for (const auto& shuffled_line : lines)
            output_file << shuffled_line << '\n';
    }

    std::cout << "Shuffled lines written to file: " << output_filename << std::endl;

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}

