CXX = g++
CXXFLAGS = -std=c++20 -Wall

LIB_NAME = libmygzip.a
LDFLAGS = -Wl,-Bstatic -lz -Wl,-Bdynamic

.PHONY: all clean

all: $(LIB_NAME) test

$(LIB_NAME): mygzip.o
	ar rcs $@ $^

test: test.o $(LIB_NAME)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(LIB_NAME) test *.o test.txt*

