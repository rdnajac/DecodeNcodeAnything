#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <zlib.h>

class GzipProcessor {
public:
    GzipProcessor(const std::string& input_file)
        : input_file_(input_file), output_file_(determine_output_filename()) {}

    bool process() {
        return is_gz() ? decompress() : compress();
    }

private:
    std::string input_file_;
    std::string output_file_;
    std::ifstream input_file_stream_;
    std::ofstream output_file_stream_;
    z_stream stream_;

    std::string determine_output_filename() const {
        return is_gz() ? input_file_.substr(0, input_file_.find_last_of(".")) : input_file_ + ".gz";
    }

    bool is_gz() const {
        return input_file_.substr(input_file_.find_last_of(".") + 1) == "gz";
    }

    bool open_files() {
        input_file_stream_.open(input_file_, std::ios::binary);
        if (!input_file_stream_.is_open()) {
            std::cerr << "Error opening input file: " << input_file_ << std::endl;
            return false;
        }

        output_file_stream_.open(output_file_, std::ios::binary);
        if (!output_file_stream_.is_open()) {
            std::cerr << "Error opening output file: " << output_file_ << std::endl;
            return false;
        }

        return true;
    }

    int initialize_zlib(int window_bits) {
        stream_.zalloc = Z_NULL;
        stream_.zfree = Z_NULL;
        stream_.opaque = Z_NULL;

        if (window_bits < 0)
            window_bits = -window_bits;

        return inflateInit2(&stream_, window_bits) != Z_OK ? Z_ERRNO : Z_OK;
    }

    bool process_input(bool compress) {
        std::vector<char> buffer(1024, '\0');

        do {
            input_file_stream_.read(buffer.data(), buffer.size());
            stream_.avail_in = static_cast<uInt>(input_file_stream_.gcount());
            stream_.next_in = reinterpret_cast<Bytef*>(buffer.data());

            int flush = compress ? Z_FINISH : Z_NO_FLUSH;

            do {
                stream_.avail_out = buffer.size();
                stream_.next_out = reinterpret_cast<Bytef*>(buffer.data());

                int result = compress ? deflate(&stream_, flush) : inflate(&stream_, flush);

                if (result == Z_STREAM_ERROR) {
                    std::cerr << "Error processing data with zlib" << std::endl;
                    return false;
                }

                output_file_stream_.write(buffer.data(), buffer.size() - stream_.avail_out);
            } while (stream_.avail_out == 0);
        } while (input_file_stream_);

        return true;
    }

    int finalize_processing() {
        return stream_.avail_out == 0 ? (stream_.avail_in == 0 ? Z_OK : Z_BUF_ERROR) : Z_STREAM_END;
    }

    bool compress() {
        return open_files() && initialize_zlib(Z_DEFAULT_COMPRESSION) == Z_OK &&
               process_input(true) && deflateEnd(&stream_) == Z_OK;
    }

    bool decompress() {
        return open_files() && initialize_zlib(-MAX_WBITS) == Z_OK &&
               process_input(false) && inflateEnd(&stream_) == Z_OK;
    }
};

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_filename>" << std::endl;
        return 1;
    }

    const std::string input_file = argv[1];

    GzipProcessor processor(input_file);
    if (!processor.process()) {
        std::cerr << "Processing failed" << std::endl;
        return 1;
    }

    return 0;
}

