#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "analysis.h"
int main( int argc, char ** argv)
{
	int c;
	char res[256];
	clock_t begin, end;
	clock_t total_time = 0;
	begin = clock();
	for(c=0; c<100000; c++) {
		get_token("int main( int argc, char ** argv)", res);
	}
	end = clock();
	total_time = end - begin;
	printf("Total time : %u\n", total_time);

	return 0;
}
