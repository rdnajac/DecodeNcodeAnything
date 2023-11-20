CC := g++
CFLAGS := -Wall -std=c++20 -O2
LDFLAGS :=


OBJECTS := test.o

all: test


test: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f test *.o a.out

.PHONY: clean
