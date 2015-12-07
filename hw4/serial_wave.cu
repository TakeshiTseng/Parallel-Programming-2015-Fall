/**********************************************************************
 * DESCRIPTION:
 *   Serial Concurrent Wave Equation - C Version
 *   This program implements the concurrent wave equation
 *********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cuda.h>

#define MAXPOINTS 1000000
#define MAXSTEPS 1000000
#define MINPOINTS 20
#define PI 3.14159265
#define FAC 6.2831853
#define SQTAU 0.09

void check_param(void);
void init_line(void);
void update (void);
void printfinal (void);
__global__ void cu_do_match();
int nsteps,                 	/* number of time steps */
    tpoints, 	     		/* total points along string */
    rcode;                  	/* generic return code */
float values[MAXPOINTS+2], 	/* values at time t */
      oldval[MAXPOINTS+2], 	/* values at time (t-dt) */
      newval[MAXPOINTS+2]; 	/* values at time (t+dt) */


/**********************************************************************
 * Checks input values from parameters
 *********************************************************************/
void check_param(void)
{
    char tchar[20];

    /* check number of points, number of iterations */
    while ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS)) {
        printf("Enter number of points along vibrating string [%d-%d]: "
                ,MINPOINTS, MAXPOINTS);
        scanf("%s", tchar);
        tpoints = atoi(tchar);
        if ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS))
            printf("Invalid. Please enter value between %d and %d\n", 
                    MINPOINTS, MAXPOINTS);
    }
    while ((nsteps < 1) || (nsteps > MAXSTEPS)) {
        printf("Enter number of time steps [1-%d]: ", MAXSTEPS);
        scanf("%s", tchar);
        nsteps = atoi(tchar);
        if ((nsteps < 1) || (nsteps > MAXSTEPS))
            printf("Invalid. Please enter value between 1 and %d\n", MAXSTEPS);
    }

    printf("Using points = %d, steps = %d\n", tpoints, nsteps);

}

/**********************************************************************
 *     Initialize points on line
 *********************************************************************/
void init_line(void)
{
    int i, j;
    float x, k, tmp;

    /* Calculate initial values based on sine curve */
    k = 0.0; 
    tmp = tpoints - 1;
    for (j = 1; j <= tpoints; j++) {
        x = k/tmp;
        values[j] = sin (FAC * x);
        k = k + 1.0;
    }

    /* Initialize old values array */
    for (i = 1; i <= tpoints; i++) 
        oldval[i] = values[i];
}

/**********************************************************************
 *      Calculate new values using wave equation
 *********************************************************************/
__global__ void cu_do_match(float* nv, float* ov, float* v, int tpoints)
{
    int i = threadIdx.x;
    if(i == 1 || i == tpoints) {
        nv[i] = 0.0;
    } else {
        nv[i] = (2.0 * v[i]) - ov[i] + (SQTAU *  (-2.0) * v[i]);
    }
    ov[i] = v[i];
    v[i] = nv[i];
}


/**********************************************************************
 *     Update all values along line a specified number of times
 *********************************************************************/
void update()
{
    int i;
    float* nv;
    float* ov;
    float* v;

    cudaMalloc((void**) &nv, MAXPOINTS + 2);
    cudaMalloc((void**) &ov, MAXPOINTS + 2);
    cudaMalloc((void**) &v, MAXPOINTS + 2);
    cudaMemcpy(nv, newval, sizeof(float)*tpoints, cudaMemcpyHostToDevice);
    cudaMemcpy(ov, oldval, sizeof(float)*tpoints, cudaMemcpyHostToDevice);
    cudaMemcpy(v, values, sizeof(float)*tpoints, cudaMemcpyHostToDevice);

    /* Update values for each time step */
    for (i = 1; i<= nsteps; i++) {
        /* Update points along line for this time step */
        cu_do_match<<<1, tpoints>>>(nv, ov, v, tpoints);
    }
    cudaMemcpy(newval, nv, sizeof(float)*tpoints, cudaMemcpyDeviceToHost);
    cudaMemcpy(oldval, ov, sizeof(float)*tpoints, cudaMemcpyDeviceToHost);
    cudaMemcpy(values, v, sizeof(float)*tpoints, cudaMemcpyDeviceToHost);

}

/**********************************************************************
 *     Print final results
 *********************************************************************/
void printfinal()
{
    int i;

    for (i = 1; i <= tpoints; i++) {
        printf("%6.4f ", values[i]);
        if (i%10 == 0)
            printf("\n");
    }
}

/**********************************************************************
 *	Main program
 *********************************************************************/
int main(int argc, char *argv[])
{
    sscanf(argv[1],"%d",&tpoints);
    sscanf(argv[2],"%d",&nsteps);
    check_param();
    printf("Initializing points on the line...\n");
    init_line();
    printf("Updating all points for all time steps...\n");
    update();
    printf("Printing final results...\n");
    printfinal();
    printf("\nDone.\n\n");

    return 0;
}
