#include "io.hpp"

std::ifstream open_file(const char* filename, std::streampos& file_size) {
    std::ifstream file(filename, std::ios::binary);

    if (file)
        file_size = std::filesystem::file_size(filename);

    return file;
}

void process_regular_file(const std::vector<char>& buffer) {
    std::cout << "Regular file content:\n" << buffer.data() << std::endl;
}

#ifdef ZLIB_FOUND
void process_gzipped_file(const std::vector<char>& buffer) {
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;

    if (inflateInit2(&stream, 16 + MAX_WBITS) != Z_OK) {
        std::cerr << "Error initializing zlib for decompression." << std::endl;
        return;
    }

    std::vector<char> decompressed_buffer;
    decompressed_buffer.resize(1024);  // Initial size

    do {
        stream.avail_out = decompressed_buffer.size();
        stream.next_out = reinterpret_cast<Bytef*>(decompressed_buffer.data());

        int ret = inflate(&stream, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            std::cerr << "Error in zlib decompression: " << ret << std::endl;
            break;
        }

        // Output the decompressed data
        std::cout.write(decompressed_buffer.data(), decompressed_buffer.size() - stream.avail_out);

        // Resize the buffer if more space is needed
        if (stream.avail_out == 0)
            decompressed_buffer.resize(decompressed_buffer.size() * 2);

    } while (stream.avail_out == 0);

    inflateEnd(&stream);
}
#else
void process_gzipped_file(const std::vector<char>& buffer) {
    std::cerr << "Error: zlib library not linked. Cannot process gzipped file." << std::endl;
}
#endif

void process_file(const char* filename) {
    std::streampos file_size;
    std::ifstream file = open_file(filename, file_size);

    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<char> buffer(file_size);
    file.read(buffer.data(), file_size);

    if (std::filesystem::path(filename).extension() == ".gz")
        process_gzipped_file(buffer);
    else
        process_regular_file(buffer);
}

