CC=clang
CFLAGS=-Wall -Wextra -Werror -Wpedantic -g -std=c11

.PHONY: all clean

all: unittests compress decompress

nibbler.o: nibbler.c nibbler.h
	$(CC) $(CFLAGS) -c nibbler.c

unittests.o: unittests.c nibbler.h
	$(CC) $(CFLAGS) -c unittests.c

compress.o: compress.c nibbler.h
	$(CC) $(CFLAGS) -c compress.c

decompress.o: decompress.c nibbler.h
	$(CC) $(CFLAGS) -c decompress.c

unittests: unittests.o nibbler.o
	$(CC) $(CFLAGS) unittests.o nibbler.o -o unittests

compress: compress.o nibbler.o
	$(CC) $(CFLAGS) compress.o nibbler.o -o compress

decompress: decompress.o nibbler.o
	$(CC) $(CFLAGS) decompress.o nibbler.o -o decompress

clean:
	rm -f *.o unittests compress decompress