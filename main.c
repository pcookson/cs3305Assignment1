#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "token.h"

#define HISTORY_SIZE 10
#define HISTORY_LINE_LENGTH 80
#define HISTORY_FILE_NAME "command_history.txt"
#define IN "<"
#define OUT ">"
#define PIPE "|"

int array_size(char *tokens)
{
    return sizeof(tokens)/sizeof(tokens[0]);
}

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

int handle_io(char *tokens[], int input_exists, int output_exists)
{
    pid_t pid;
    int fd;
    //char *outputFile;
    //strncpy(tokens[4], tokens[4], strlen(tokens[4])-2);
    //strcat(tokens[4], "\0");
    //printf("%s", outputFile);
    printf("%s", tokens[4]);
    pid = fork();

    if(pid<0)
    {
        perror("fork()");
    }

    if(pid>0)
    {
        //parent

        wait(NULL);


    }
    else if(pid==0)
    {
        if(input_exists == 1)
        {
            fd = open(tokens[2], O_RDONLY, 0);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if(output_exists == 1)
        {



            fd = open(tokens[4], O_WRONLY|O_CREAT|O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execlp(tokens[0], tokens[0], NULL);
        perror("execlp");
    }

}

int execute_binary(char *tokens[], int tokenSize)
{

    pid_t pid;

    pid = fork();

    if(pid<0)
    {
        perror("fork()");
    }

    if(pid>0)
    {
        //parent


        wait(NULL);


    }
    else if(pid==0)
    {
        int index;
        char *newTokens[tokenSize];



        execvp(tokens[0], tokens);
        perror("execvp");
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
    char *username;
    FILE *historyFile;

    int IN_exists;
    int OUT_exists;
    int PIPE_exists;

    username = getlogin();
    //execution loop
    while(1)
    {
        IN_exists = 0;
        OUT_exists = 0;
        PIPE_exists = 0;

        printf("%s> ", username);
        //char* inp = fgets(input_line, MAX, stdin);
        if(fgets(input_line, MAX, stdin) != NULL)
        //if(strlen(input_line) != 0 || inp != NULL)
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
            //printf("in exit if\n");
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

        }

        if(n != 0)
        {
            index=0;
            while(tokens[index]!=NULL)
            {

                if(strcmp(tokens[index], OUT) == 0)
                {
                    OUT_exists = 1;
                }
                else if(strcmp(tokens[index], IN) == 0)
                {
                    IN_exists = 1;

                }
                else if(tokens[index] == PIPE)
                {
                    PIPE_exists = 1;
                }
                index++;
            }

            if(OUT_exists == 1 || IN_exists == 1 && PIPE_exists == 0)
            {

                handle_io(tokens, IN_exists, OUT_exists);
            }
            else
            {
                execute_binary(tokens, index);
            }
        }

    }//endwhile
}
