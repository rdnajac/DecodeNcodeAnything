// main.cpp
#include "../src/codec.cpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    Codec codec(filename);

    codec.print_info();
    codec.encode();
    codec.oligodump();

    return 0;
}

