CC := g++
CFLAGS := -Wall -std=c++20 -O2 
LDFLAGS :=


OBJECTS := revcom.o

all: revcom


revcom: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f revcom *.o a.out

.PHONY: clean
