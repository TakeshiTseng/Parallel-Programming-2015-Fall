#include <stdio.h>
#include <string.h>
#include "analysis.h"
#include <omp.h>
#include <time.h>
#define NUM_THREADS 1

int main(int argc, const char *argv[])
{
	if(argc != 2) {
		return -1;
	}
	char filename[256];
	strncpy(filename, argv[1], 256);
    clock_t start,end,total;
	FILE* f = fopen(filename, "r");
	char line[256] = {0};
	char res[256] = {0};
	int tkn_list[20] = {0};
    int tkn_split[NUM_THREADS][20]={0};
    int linecount=0;
    int is_in_comment = 0;
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        int id, nthrds;
        id = omp_get_thread_num();
        nthrds= omp_get_num_threads();
//        printf("id = %d\nnthrds = %d\n",id,nthrds);
        is_in_comment = 0;
        int ln = 1;
//        printf("id = %d\n",id);
        start = clock();
        while(fgets(line, 256, f) != NULL) {
            linecount++;
            int tkn = get_token(line, res);
            if(is_in_comment == 0) {
                tkn_split[id][tkn]++;
            } else {
                tkn_split[id][COMMENT]++;
            }
            if(tkn == COMMENT_START) {
                is_in_comment = 1;
            }
            if(tkn == COMMENT_END) {
                is_in_comment = 0;
            }
            if(is_in_comment ==1){
                tkn = 7;
            }
//            printf("%d %d %s", ln++, tkn, line);
        }
    }
    int i,j;
//    for(i=0 ; i<NUM_THREADS;i++){
//        sum+= linecount[i];
//    }
    for(i=0;i<20;i++){
        for(j=0;j<NUM_THREADS;j++){
            tkn_list[i]+=tkn_split[j][i];
        }
    }
    end = clock();
    /*printf("linecount = %d\n", linecount);
	printf("func def : %d\n", tkn_list[0]);
	printf("func call : %d\n", tkn_list[1]);
	printf("var def : %d\n", tkn_list[2]);
	printf("if : %d\n", tkn_list[3]);
	printf("else if : %d\n", tkn_list[4]);
	printf("else : %d\n", tkn_list[5]);
	printf("while : %d\n", tkn_list[6]);
	printf("comment : %d\n", tkn_list[7]);
	printf("include : %d\n", tkn_list[10]);
	printf("ignore : %d\n", tkn_list[11]);
	printf("return : %d\n", tkn_list[12]);
	printf("array def : %d\n", tkn_list[13]);
	printf("func imp : %d\n", tkn_list[14]);
    printf("struct : %d\n", tkn_list[15]);
    printf("assign : %d\n", tkn_list[16]);*/
    total = end-start;
    printf("Total time: %u\n", total);
    fclose(f);
	return 0;
}
