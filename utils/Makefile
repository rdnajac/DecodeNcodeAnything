CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
LDFLAGS = -lz

.PHONY: all clean

all: gzip_processor gen_fastq randomizer

gzip_processor: gzip_processor.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f gzip_processor *.o

