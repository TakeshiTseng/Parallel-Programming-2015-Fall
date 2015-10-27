#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

#define TOTAL_POINT 1000000000
#define NUM_THREAD 1


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
    pthread_t thread[NUM_THREAD];
    srand(time(NULL));

    int tmp = 0;
    int c;
    for(c=0; c<NUM_THREAD; c++) {
        int* arg = malloc(sizeof(int));
        *arg = TOTAL_POINT / NUM_THREAD;
        if(c == NUM_THREAD - 1) {
            *arg = TOTAL_POINT - tmp;
        }
        tmp += *arg;
        pthread_create(&thread[c], NULL, cal_thread, arg);
    }
    unsigned long long total_point_in_circle = 0;
    for(c=0; c<NUM_THREAD; c++) {
        void* retval;
        pthread_join(thread[c], &retval);
        total_point_in_circle += *((unsigned long long*)retval);
    }
    printf("PI : %f\n", (double)total_point_in_circle*4 / TOTAL_POINT);
    return 0;
}
