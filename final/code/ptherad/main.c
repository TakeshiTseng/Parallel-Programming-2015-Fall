#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "analysis.h"

char lines[850100][256];
int num_thread = 8;
int num_lines = 0;
int result[8][20];

void* analy_thread(void* arg) {
	int tid = *((int*) arg);
	int start, end;
	int width = num_lines / num_thread;
	start = width * tid;
	end = width * (tid + 1);
	int c;
	int is_in_comment = 0;
	char res[256];
	char line[256];
	for(c=start; c<end; c++) {
		strncpy(line, lines[c], 256);
		int tkn = get_token(line, res);
		if(is_in_comment == 0) {
			result[tid][tkn]++;
		} else {
			result[tid][COMMENT]++;
		}
		if(tkn == COMMENT_START) {
			is_in_comment = 1;
		}
		if(tkn == COMMENT_END) {
			is_in_comment = 0;
		}
		if(is_in_comment == 1) {
			tkn = 7;
		}
	}
	return NULL;
}


int main(int argc, const char *argv[])
{
	char filename[256];
	char line[256];
	strncpy(filename, argv[1], 256);
	pthread_t* thread = malloc(sizeof(pthread_t) * num_thread);
	FILE* f = fopen(filename, "r");
	while(fgets(line, 256, f) != NULL) {
		strncpy(lines[num_lines], line, 256);
		num_lines++;
	}
	fclose(f);
	int r, c;
	for(r=0; r<8; r++) {
		for(c=0; c<20; c++) {
			result[r][c] = 0;
		}
	}
	build_re();
	clock_t begin, end;
	clock_t total_time = 0;
	begin = clock();
	for(c=0; c<num_thread; c++) {
		int* arg = malloc(sizeof(int));
		*arg = c; // tid
		pthread_create(&thread[c], NULL, analy_thread, arg);
	}
	for(c=0; c<num_thread; c++) {
		void* retval;
		pthread_join(thread[c], &retval);
	}
	end = clock();
	total_time = end - begin;
	printf("Total time : %u\n", total_time);
	return 0;
}
