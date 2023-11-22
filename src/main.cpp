#include "oligo.hpp"
#include "utils.hpp"

int main() {
    std::string dna = "ATCG";
    std::string revcom_dna = revcom(dna);
    std::cout << "Original DNA: " << dna << std::endl;
    std::cout << "Reverse Complement: " << revcom_dna << std::endl;

    std::cout << "Hello, World!" << std::endl;

    return 0;
}

