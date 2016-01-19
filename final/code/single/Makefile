CC=cc
CFLAG=-O3 -g -I/usr/local/include -L/usr/local/lib -lpcre
all:
	$(CC) $(CFLAG) analysis.c main.c -o main
test:
	$(CC) $(CFLAG) analysis.c test.c -o test
