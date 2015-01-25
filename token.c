#include "token.h"

int make_tokenlist(char *buf, char *tokens[])
{
    char historyLine[100];
    char *line;
    int i,n;

    i=0;

    line = buf;
    if(strncmp(line, "\n", 1)!=0)
    //if(line !=NULL);
    {
        tokens[i] = strtok(line, " \n");

        do
        {
            i++;
            line = NULL;
            tokens[i] = strtok(line, " \n");
        }
        while(tokens[i] != NULL);
    }

    return i;
}
