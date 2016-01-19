#include <time.h>
#include <stdio.h>
#include <string.h>
#include "analysis.h"


int main(int argc, const char *argv[])
{
	if(argc != 2) {
		return -1;
	}
	clock_t begin, end;
	clock_t time_spent;
	clock_t total_time = 0;
	char filename[256];

	strncpy(filename, argv[1], 256);
	FILE* f = fopen(filename, "r");
	char line[256] = {0};
	char res[256] = {0};
	int tkn_list[20] = {0};

	int is_in_comment = 0;
	int ln = 1;
	while(fgets(line, 256, f) != NULL) {
		begin = clock();

		int tkn = get_token(line, res);

		if(is_in_comment == 0) {
			tkn_list[tkn]++;
		} else {
			tkn_list[COMMENT]++;
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
		//		printf("%d %d %s", ln++, tkn, line);
		end = clock();
		time_spent = end - begin;
		total_time += time_spent;
	}

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
	printf("assign : %d\n", tkn_list[16]);

	printf("Total time : %u\n", total_time);

	fclose(f);
	return 0;
}
