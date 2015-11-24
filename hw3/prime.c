#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int isprime(int n) {
    int i,squareroot;
    if (n>10) {
        squareroot = (int) sqrt(n);
        for (i=2; i<=squareroot; i++) {
            if ((n % i) == 0) {
                return 0;
            }
        }
        return 1;
    } else if (n == 2 || n == 3 || n == 5 || n == 7) {
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int pc,       /* prime counter */
        foundone; /* most recent prime found */
    int sendbuf[2]; // pc and foundone.
    int* rvbuf;
    long long int n, limit;
    long long int start, end;
    long long int num_each_node;

    sscanf(argv[1],"%llu",&limit);

    if(rank == 0) {
        // let master to print message
        printf("Starting. Numbers to be scanned= %lld\n",limit);

        // init receive buffer
        rvbuf = malloc(2 * size * sizeof(int));
    }

    pc=0;

    num_each_node = limit / size;
    if(rank == size - 1) {
        start = rank * num_each_node;
        end = limit + 1;
    } else {
        start = rank * num_each_node;
        end = start + num_each_node;
    }

    // calculate prime here
    for(n=start; n<end; n++) {
        if(isprime(n) == 1) {
            pc++;
            foundone = n;
        }
    }

    sendbuf[0] = pc;
    sendbuf[1] = foundone;

    MPI_Barrier(MPI_COMM_WORLD);

    int result_pc = 0;
    int result_fo = 0;
    MPI_Reduce(&pc, &result_pc, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&foundone, &result_fo, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0) {
        printf("Done. Largest prime is %d Total primes %d\n",result_fo, result_pc);
    }
    MPI_Finalize();
    return 0;
}

