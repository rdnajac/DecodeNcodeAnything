#include "io.hpp"

// Check if zlib is available
#ifdef ZLIB_FOUND
#include <zlib.h>
#endif

int main() {
    const char* regular_file = "regular.txt";
    const char* gzipped_file = "gzipped.gz";

    std::ofstream regular_out(regular_file, std::ios::binary);
    regular_out << "This is a regular file." << std::endl;
    regular_out.close();

    // Create a gzipped file only if zlib is available
    #ifdef ZLIB_FOUND
    std::ofstream gzipped_out(gzipped_file, std::ios::binary);
    gzFile gzfile = gzopen(gzipped_file, "wb");
    if (gzfile != NULL) {
        gzprintf(gzfile, "This is a gzipped file.");
        gzclose(gzfile);
    }
    gzipped_out.close();
    #endif

    std::cout << "Processing regular file:\n";

    #ifdef ZLIB_FOUND
    std::cout << "\nProcessing gzipped file:\n";
    process_file(gzipped_file);
    #else
    std::cout << "\nZlib not available. Skipped processing gzipped file.\n";
    #endif

    return 0;
}

