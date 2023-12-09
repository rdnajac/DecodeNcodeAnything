#include "../src/codec.cpp"
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    const std::string filename = argv[1];
    Codec codec(filename);
    codec.print_info();
    auto start_time = std::chrono::high_resolution_clock::now();
    codec.encode();
    auto end_time = std::chrono::high_resolution_clock::now();
    //codec.oligodump();
    codec.write_duplex();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout <<  "Elapsed Time " << duration.count() << " ms" << std::endl;

    return 0;
}

