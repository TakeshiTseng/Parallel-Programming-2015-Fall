CC=gcc
CFLAG=-g -O3 -fopenmp -I/usr/local/include -L/usr/local/lib -lpcre
all:
	$(CC) $(CFLAG) analysis.c main.c -o main
c:
	rm -f main
	rm -f main.core
