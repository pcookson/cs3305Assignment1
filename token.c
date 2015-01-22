#include "token.h"

int make_tokenlist(char *buf, char *tokens[])
{
	char historyLine[100];
    char *line;
    int i,n;

    i=0;

    line = buf;
    tokens[i] = strtok(line, " ");

    do
    {
        i++;
        line = NULL;
        tokens[i] = strtok(line, " ");
    }
    while(tokens[i] != NULL);

    return i;
}
