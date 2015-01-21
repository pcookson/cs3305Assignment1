#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 256
#define CMD_MAX 10


int make_tokenlist(char *buf, char *tokens[]);

#endif // TOKEN_H_INCLUDED
