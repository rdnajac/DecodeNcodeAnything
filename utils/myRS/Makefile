CXX = g++
CXXFLAGS = -std=c++20 -Wall

TARGET = testRS
SOURCE = ReedSolomonTest.cpp myRSmodule.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean

