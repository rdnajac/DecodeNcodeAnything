CXX = g++
CXXFLAGS = -std=c++20 -Wall -O2

SRCS = oligo.cpp test.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = test

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

