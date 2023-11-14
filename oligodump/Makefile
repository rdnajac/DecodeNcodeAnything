CC := g++
CFLAGS := -Wall -std=c++20 -O2
LDFLAGS :=


OBJECTS := oligodump.o

all: oligodump


oligodump: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f oligodump *.o a.out

.PHONY: clean
