#include<stdlib.h>
#include<pcre.h>
#include<string.h>
#include"analysis.h"

char* ex_return = "return";
char* ex_include = "#include";
char* ex_if_stat = "if\\(.*\\)";
char* ex_elif_stat = "else if\\(.*\\)";
char* ex_else_stat = "else";
char* ex_func_def = "[a-zA-Z0-9_]+[ ][a-zA-Z_][a-zA-Z0-9_]+\\(.*\\);";
char* ex_func_imp = "[a-zA-Z0-9_]+[ ][a-zA-Z_][a-zA-Z0-9_]+\\(.*\\)[ ]?\\{?";
char* ex_func_call = "[a-zA-Z_][a-zA-Z0-9_]+\\(.*\\);";
char* ex_var_def = "[a-zA-Z0-9_]+[ ][a-zA-Z_][a-zA-Z0-9_]+;";
char* ex_while_stat = "while\\(.*\\)";
char* ex_comment = "[/][/]";
char* ex_comment_start = "/\\*";
char* ex_comment_end = "\\*/";
char* ex_array_def = "[a-zA-Z0-9_]+[ ][a-zA-Z_][a-zA-Z0-9_]+\\[[0-9a-zA-Z_]*\\];";
char* ex_struct = "struct [a-zA-Z_][a-zA-Z0-9_]+ \\{";
char* ex_assign = "[^=]*=[^=]*";


pcre* re_return;
pcre* re_include;
pcre* re_if_stat;
pcre* re_elif_stat;
pcre* re_else_stat;
pcre* re_func_def;
pcre* re_func_imp;
pcre* re_func_call;
pcre* re_var_def;
pcre* re_while_stat;
pcre* re_comment;
pcre* re_comment_start;
pcre* re_comment_end;
pcre* re_array_def;
pcre* re_struct;
pcre* re_assign;

void build_re() {
	const char* error;
	int erroffset;
	int options = 0;

	re_return = pcre_compile("return", options, &error, &erroffset, NULL);
	re_include = pcre_compile("#include", options, &error, &erroffset, NULL);
	re_if_stat = pcre_compile("if\\(.*\\)", options, &error, &erroffset, NULL);
	re_elif_stat = pcre_compile("else if\\(.*\\)", options, &error, &erroffset, NULL);
	re_else_stat = pcre_compile("else", options, &error, &erroffset, NULL);
	re_func_def = pcre_compile("[a-zA-Z0-9_]+[ ][a-zA-Z_][a-zA-Z0-9]+\\(.*\\);", options, &error, &erroffset, NULL);
	re_func_imp = pcre_compile("[a-zA-Z0-9_]+[ ][a-zA-Z_][a-zA-Z0-9]+\\(.*\\)[ ]?\\{?", options, &error, &erroffset, NULL);
	re_func_call = pcre_compile("[a-zA-Z_][a-zA-Z0-9_]+\\(.*\\);", options, &error, &erroffset, NULL);
	re_var_def = pcre_compile("[a-zA-Z0-9_]+[ ][a-zA-Z_][a-zA-Z0-9_+;", options, &error, &erroffset, NULL);
	re_while_stat = pcre_compile("while\\(.*\\)", options, &error, &erroffset, NULL);
	re_comment = pcre_compile("[/][/]", options, &error, &erroffset, NULL);
	re_comment_start = pcre_compile("/\\*", options, &error, &erroffset, NULL);
	re_comment_end = pcre_compile("\\*/", options, &error, &erroffset, NULL);
	re_array_def = pcre_compile("[a-zA-Z0-9_]+[ ][a-zA-Z_][a-zA-Z0-_]+\\[[0-9a-zA-Z_]*\\];", options, &error, &erroffset, NULL);
	re_struct = pcre_compile("struct [a-zA-Z_][a-zA-Z0-9_]+ \\{", options, &error, &erroffset, NULL);
	re_assign = pcre_compile("[^=]*=[^=]*", options, &error, &erroffset, NULL);

}

void strip_line(char* line) {
	while(*line == '\t' || *line == ' ') {
		char* tmp = line;
		while((*tmp == '\t' || *tmp == ' ') && *tmp != '\0') {
			tmp[0] = tmp[1];
			tmp++;
		}
	}
}

int get_token(char* line, char text_res[256]) {
	char* tmp_line = malloc(sizeof(char) * strlen(line));
	strncpy(tmp_line, line, strlen(line));
	strip_line(tmp_line);
	strncpy(text_res, tmp_line, strlen(tmp_line));
	if(is_match(tmp_line, re_include) == 1) {
		return INCLUDE_STMT;
	} else if(is_match(tmp_line, re_return) == 1) {
		return RETURN_STMT;
	} else if(is_match(tmp_line, re_while_stat) == 1) {
		return WHILE_STAT;
	} else if(is_match(tmp_line, re_if_stat) == 1) {
		return IF_STAT;
	} else if(is_match(tmp_line, re_elif_stat) == 1) {
		return ELIF_STAT;
	} else if(is_match(tmp_line, re_elif_stat) == 1) {
		return ELSE_STAT;
	} else if(is_match(tmp_line, re_struct) == 1) {
		return STRUCT;
	} else if(is_match(tmp_line, re_assign) == 1) {
		return ASSIGN;
	}else if(is_match(tmp_line, re_func_def) == 1) {
		return FUNC_DEF;
	}else if(is_match(tmp_line, re_func_imp) == 1) {
		return FUNC_IMP;
	} else if(is_match(tmp_line, re_func_call) == 1) {
		return FUNC_CALL;
	} else if(is_match(tmp_line, re_array_def) == 1) {
		return ARRAY_DEF;
	} else if(is_match(tmp_line, re_var_def) == 1) {
		return VAR_DEF;
	} else if(is_match(tmp_line, re_comment) == 1) {
		return COMMENT;
	} else if(is_match(tmp_line, re_comment_end) == 1) {
		return COMMENT_END;
	} else if(is_match(tmp_line, re_comment_start) == 1) {
		return COMMENT_START;
	} else {
		return IGNORE;
	}

	free(tmp_line);
}


/**
Match: 1
Not match: 0
 **/
	int is_match(const char* str, pcre* re) {
		const char* error;
		int erroffset;
		int ovector[10];
		int options = 0;

		if(pcre_exec(re, NULL, str, strlen(str), 0, 0, ovector, 10) >= 0) {
			return 1;
		} else {
			return 0;
		}
	}

