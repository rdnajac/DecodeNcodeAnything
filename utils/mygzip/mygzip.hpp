#ifndef MYGZIP_HPP
#define MYGZIP_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <zlib.h>

class Gzipper {
public:
    Gzipper(const std::string& input_file)
        : input_file_(input_file),
          output_file_(determine_output_filename()),
          input_file_stream_(input_file, std::ios::binary),
          output_file_stream_(output_file_, std::ios::binary | std::ios::trunc) {}

    bool process() {
        std::cerr << "Processing file: " << input_file_ << std::endl;
        return is_gz() ? decompress() : compress();
    }

private:
    static const std::size_t BUFFER_SIZE = 1024;

    std::string input_file_;
    std::string output_file_;
    std::ifstream input_file_stream_;
    std::ofstream output_file_stream_;
    z_stream stream_;

    std::string determine_output_filename() const {
        return is_gz() ? input_file_.substr(0, input_file_.find_last_of(".")) : input_file_ + ".gz";
    }

    bool is_gz() const {
        return input_file_.substr(input_file_.find_last_of(".")) == ".gz";
    }

    bool open_files() {
        input_file_stream_.open(input_file_, std::ios::binary);
        if (!input_file_stream_.is_open()) {
            log_error("Error opening input file: " + input_file_);
            return false;
        }

        output_file_stream_.open(output_file_, std::ios::binary | std::ios::trunc);
        if (!output_file_stream_.is_open()) {
            log_error("Error opening output file: " + output_file_);
            return false;
        }

        return true;
    }

    int initialize_zlib(int window_bits) {
        std::cerr << "Initializing zlib with window_bits: " << window_bits << std::endl;

        stream_.zalloc = Z_NULL;
        stream_.zfree = Z_NULL;
        stream_.opaque = Z_NULL;

        if (window_bits < 0)
            window_bits = -window_bits;

        return inflateInit2(&stream_, window_bits) != Z_OK ? Z_ERRNO : Z_OK;
    }

    int finalize_processing() {
        std::cerr << "Finalizing processing..." << std::endl;

        return stream_.avail_out == 0 ? (stream_.avail_in == 0 ? Z_OK : Z_BUF_ERROR) : Z_STREAM_END;
    }

    void log_error(const std::string& message) {
        std::cerr << "Error: " << message << std::endl;
    }

    bool compress() {
        if (open_files() && initialize_zlib(Z_DEFAULT_COMPRESSION) == Z_OK) {
            std::vector<char> buffer(BUFFER_SIZE, '\0');

            do {
                input_file_stream_.read(buffer.data(), buffer.size());
                stream_.avail_in = static_cast<uInt>(input_file_stream_.gcount());
                stream_.next_in = reinterpret_cast<Bytef*>(buffer.data());

                int flush = Z_FINISH;

                do {
                    stream_.avail_out = buffer.size();
                    stream_.next_out = reinterpret_cast<Bytef*>(buffer.data());

                    int result = deflate(&stream_, flush);

                    if (result == Z_STREAM_ERROR) {
                        log_error("Error processing data with zlib");
                        return false;
                    }

                    output_file_stream_.write(buffer.data(), buffer.size() - stream_.avail_out);
                } while (stream_.avail_out == 0);
            } while (input_file_stream_);

            int deflateEndResult = deflateEnd(&stream_);
            if (deflateEndResult == Z_OK) {
                std::cerr << "Compression successful." << std::endl;
                return true;
            } else {
                log_error("deflateEnd failed: " + std::string(zError(deflateEndResult)));
            }
        } else {
            log_error("Initialization failed.");
        }
        return false;
    }

    bool decompress() {
        std::cerr << "Decompression..." << std::endl;

        if (open_files() && initialize_zlib(-MAX_WBITS) == Z_OK) {
            std::vector<char> buffer(BUFFER_SIZE, '\0');

            do {
                input_file_stream_.read(buffer.data(), buffer.size());
                stream_.avail_in = static_cast<uInt>(input_file_stream_.gcount());
                stream_.next_in = reinterpret_cast<Bytef*>(buffer.data());

                int flush = Z_FINISH;

                do {
                    stream_.avail_out = buffer.size();
                    stream_.next_out = reinterpret_cast<Bytef*>(buffer.data());

                    int result = inflate(&stream_, flush);

                    if (result == Z_STREAM_ERROR) {
                        log_error("Error processing data with zlib");
                        return false;
                    }

                    output_file_stream_.write(buffer.data(), buffer.size() - stream_.avail_out);
                } while (stream_.avail_out == 0);
            } while (input_file_stream_);

            int inflateEndResult = inflateEnd(&stream_);
            if (inflateEndResult == Z_OK) {
                std::cerr << "Decompression successful." << std::endl;
                return true;
            } else {
                log_error("inflateEnd failed: " + std::string(zError(inflateEndResult)));
            }
        } else {
            log_error("Initialization failed.");
        }
        return false;
    }
};

#endif  // MYGZIP_HPP

