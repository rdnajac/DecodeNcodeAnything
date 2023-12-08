#include "../src/codec.cpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{

    if (argc != 2)
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl, exit(1);

    const std::string filename = argv[1];

    Codec codec(filename);

        std::cerr << "Error opening file: " << filename << ". Please try again." << std::endl;

    codec.print_info();
    codec.encode();
    codec.write_duplex();
    std::cout << "Outfile created!\n";
    return 0;
}

