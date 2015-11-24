#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int isprime(int n) {
    int i,squareroot;
    if (n>10) {
        squareroot = (int) sqrt(n);
        for (i=3; i<=squareroot; i=i+2)
            if ((n%i)==0)
                return 0;
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
    long long int n, limit;
    long long int start, end;

    sscanf(argv[1],"%llu",&limit);

    if(rank == 0) {
        // let master to print message
        printf("Starting. Numbers to be scanned= %lld\n",limit);
    }

    pc=0;

    for (n=11; n<=limit; n=n+2) {
        if (isprime(n)) {
            pc++;
            foundone = n;
        }
    }

    if(rank == 0) {
        // reduce slaves result
        printf("[%d/%d] Done. Largest prime is %d Total primes %d\n",rank + 1, size, foundone,pc);
    } else {
        // send result to master
    }
    MPI_Finalize ();
    return 0;
}

