#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>



void* cal_thread(void* arg) {
    int np = *((int*) arg);
    int c;
    unsigned long long* point_in_circle = malloc(sizeof(unsigned long long));
    *point_in_circle = 0;
    for(c=0; c<np; c++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        double r2 = x*x + y*y;
        if(r2 <= 1) {
            *point_in_circle = *point_in_circle + 1;
        }
    }
    return (void*) point_in_circle;
}

int main(int argc, const char *argv[])
{
    int num_thread = get_nprocs();
    int total_points = 0;
    int c;
    int tmp = 0;
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
    srand(time(NULL));

    for(c=0; c<num_thread; c++) {
        int* arg = malloc(sizeof(int));
        *arg = total_points / num_thread;
        if(c == num_thread - 1) {
            *arg = total_points - tmp;
        }
        tmp += *arg;
        pthread_create(&thread[c], NULL, cal_thread, arg);
    }
    unsigned long long total_point_in_circle = 0;
    for(c=0; c<num_thread; c++) {
        void* retval;
        pthread_join(thread[c], &retval);
        total_point_in_circle += *((unsigned long long*)retval);
    }
    printf("PI : %f\n", (double)total_point_in_circle*4 / total_points);
    return 0;
}
