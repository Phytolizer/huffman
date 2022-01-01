CFLAGS ?= -Wall -std=c11 -Wpedantic -Iinclude
CC ?= gcc

all: libcompress.a

libcompress.a: source/compress.o source/state.o source/node.o source/sort.o source/build.o
	ar rcs $@ $^

source/compress.o: source/compress.c
	$(CC) $(CFLAGS) -c $< -o $@

source/state.o: source/state.c
	$(CC) $(CFLAGS) -c $< -o $@

source/node.o: source/node.c
	$(CC) $(CFLAGS) -c $< -o $@

source/sort.o: source/sort.c
	$(CC) $(CFLAGS) -c $< -o $@

source/build.o: source/build.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -vf source/*.o *.a
