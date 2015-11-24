#include <mpi.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int size, rank;
    long long i, num_intervals, start, end, num_each_node;
    double rect_width, area, sum, x_middle;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    sscanf(argv[1], "%llu", &num_intervals);
    rect_width = PI / num_intervals;

    num_each_node = num_intervals / size;
    if(rank == size - 1) {
        start = num_each_node * rank + 1;
        end = num_intervals + 1;
    } else {
        start = num_each_node * rank + 1;
        end = start + num_each_node;
    }

    sum = 0;
    for(i = start; i < end; i++) {
        x_middle = (i - 0.5) * rect_width;
        area = sin(x_middle);
        sum = sum + area;
    }
    sum = sum * rect_width;

    double result_sum;
    MPI_Reduce(&sum, &result_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0) {
        printf("The total area is: %f\n", (float)result_sum);
    }
    MPI_Finalize();
    return 0;
}

