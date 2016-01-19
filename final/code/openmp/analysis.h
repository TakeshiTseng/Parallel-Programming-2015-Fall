#ifndef _ANAL_H_
#define _ANAL_H_
#include <pcre.h>

#define FUNC_DEF 0
#define FUNC_CALL 1
#define VAR_DEF 2
#define IF_STAT 3
#define ELIF_STAT 4
#define ELSE_STAT 5
#define WHILE_STAT 6
#define COMMENT 7
#define COMMENT_START 8
#define COMMENT_END 9
#define INCLUDE_STMT 10
#define IGNORE 11
#define RETURN_STMT 12
#define ARRAY_DEF 13
#define FUNC_IMP 14
#define STRUCT 15
#define ASSIGN 16

int is_match(const char* str, pcre* re);
int get_token(char* line, char text_res[256]);
void strip_line(char* line);
void build_re();

#endif
