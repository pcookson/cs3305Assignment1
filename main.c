#include <stdio.h>
#include <stdlib.h>
#include "token.h"

#define HISTORY_SIZE 10
#define HISTORY_LINE_LENGTH 80
#define HISTORY_FILE_NAME "command_history.txt"

int file_exists(const char *fname)
{
    FILE *file;
    if(file = fopen(fname, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int print_n_history(int arrayLength, int numHistory, FILE *file)
{
    char *historyArray[arrayLength];
    char line[HISTORY_LINE_LENGTH];
    char *copyString;
    int index;
    for(index=0; index<arrayLength; index++)
    {
        fgets(line, HISTORY_LINE_LENGTH, file);
        copyString = strdup(line);
        historyArray[index] = copyString;
    }
    for(index=arrayLength-numHistory; index<arrayLength; index++)
    {
        printf("%s", historyArray[index]);
    }
}

int main()
{
    char input_line[MAX], *tokens[CMD_MAX];
    int i, n;
    int index;
    int historyIndex = 0;
    char line[HISTORY_LINE_LENGTH];
    char *historyString;
    FILE *historyFile;

    //execution loop
    while(1)
    {
        printf("patrick> ");

        if(fgets(input_line, MAX, stdin) != NULL)
        {
            if((historyString = strdup(input_line))!=NULL)
            {
                historyIndex++;

                historyFile = fopen(HISTORY_FILE_NAME, "a");

                fprintf(historyFile, "%s", historyString);

                fclose(historyFile);
            }

            n = make_tokenlist(input_line, tokens);

        }
        else
        {
            printf("huh?\n");
        }

        if(strncmp(tokens[0], "exit", 4) == 0 && n==1)
        {
            remove(HISTORY_FILE_NAME);
            return 0;

        }
        else if(strncmp(tokens[0], "exit", 4) == 0 && n!=1)
        {
            printf("'exit' takes 0 parameters\n");

        }
        else if(strncmp(tokens[0], "history", 7) == 0 && n==1)
        {
            historyFile = fopen(HISTORY_FILE_NAME, "rt");
            if(historyIndex<10)
            {
                print_n_history(historyIndex, historyIndex, historyFile);
                fclose(historyFile);
            }
            else
            {
                print_n_history(historyIndex, HISTORY_SIZE, historyFile);
                fclose(historyFile);
            }

        }
        else if(strncmp(tokens[0], "history", 7) == 0 && n==2)
        {
            if(file_exists(HISTORY_FILE_NAME))
            {
                historyFile = fopen(HISTORY_FILE_NAME, "rt");
                if(historyIndex<atoi(tokens[1]))
                {
                    printf("Not enough history for %d entries.\n", atoi(tokens[1]));
                    print_n_history(historyIndex, historyIndex, historyFile);

                }
                else
                {
                    print_n_history(historyIndex, atoi(tokens[1]), historyFile);
                }




            }
            else if(strncmp(tokens[0], "history", 7) == 0 && n!=1)
            {
                printf("history take 0 parameters\n");
                break;
            }
        }


    }
}
