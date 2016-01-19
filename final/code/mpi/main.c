#include <stdio.h>
#include <string.h>
#include "analysis.h"
#include <time.h>
#include "analysis.h"


int main(int argc, const char *argv[])
{
	MPI_Init(&argc, &argv);

	int my_id, num_process;
	
	int i;

	if(argc != 2) {
		return -1;
	}
	char filename[256];

	strncpy(filename, argv[1], 256);
	FILE* f = fopen(filename, "r");
	char line[256] = {0};
	char res[256] = {0};
	int tkn_list[15] = {0};

	int is_in_comment = 0;
	int ln = 1;
	
	int num_of_line = 0;
	
	while(fgets(line, 256, f) != NULL) {
		num_of_line++;	
	}
	
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_process);

	int start = (num)
	
	while(fgets(line, 256, f) != NULL) {
		int tkn = get_token(line, res);
		//printf("%d %d %s", ln++, tkn, line);
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
	fclose(f);
	return 0;
}
