# Author: Tseng Yi
CC=mpicc
MASTER=10.10.0.173
SLAVE1=10.10.0.174
SLAVE2=10.10.0.175
SLAVE3=10.10.0.176

clean:
	rm -rf ./bin/*
prime:
	$(CC) prime.c -lm -o ./bin/prime

integrate:
	$(CC) integrate.c -lm -o ./bin/integrate

run_prime:
	mpirun -host $(MASTER),$(SLAVE1),$(SLAVE2),$(SLAVE3) ./bin/prime 1000
