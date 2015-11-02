#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>

unsigned long long* points_in_circle;
int total_points = 0;
int num_thread = 1;

void* cal_thread(void* arg) {
    int tid = *((int*) arg);
    int *seed = malloc(sizeof(int));
    *seed = ((int)time(NULL) ^ tid);
    int c;
    int np = total_points / num_thread;
    points_in_circle[tid] = 0;
    for(c=0; c<np; c++) {
        double x = (double)rand_r(arg) / RAND_MAX;
        double y = (double)rand_r(arg) / RAND_MAX;
        double r2 = x*x + y*y;
        if(r2 <= 1) {
            ++points_in_circle[tid];
        }
    }
    return NULL;
}

int main(int argc, const char *argv[])
{
    int c;
    int tmp = 0;
    num_thread = get_nprocs();
    if(argc < 2) {
        printf("usage: ./hw1 number-of-tosses\n");
        return -1;
    } else {
        total_points = atoi(argv[1]);
    }
    if(total_points <= 0) {
        printf("num of tosses should be larger than 0");
        return -1;
    }

    pthread_t* thread = malloc(sizeof(pthread_t) * num_thread);
    points_in_circle = malloc(sizeof(unsigned long long) * num_thread);

    for(c=0; c<num_thread; c++) {
        int* arg = malloc(sizeof(int));
        *arg = c; // tid
        tmp += total_points / num_thread;
        pthread_create(&thread[c], NULL, cal_thread, arg);
    }
    total_points = tmp;

    unsigned long long total_point_in_circle = 0;
    for(c=0; c<num_thread; c++) {
        void* retval;
        pthread_join(thread[c], &retval);
        total_point_in_circle += points_in_circle[c];
    }
    printf("%.15f\n", (double)total_point_in_circle*4 / total_points);
    return 0;
}
