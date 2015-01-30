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

int handle_io(char *tokens[], int input_exists, int output_exists
              ,int input_token_location, int output_token_location)
{
    pid_t pid;
    int fd;
    pid = fork();

    if(pid<0)
    {
        perror("fork()");
        exit(-1);
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
            fd = open(tokens[input_token_location], O_RDONLY, 0);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if(output_exists == 1)
        {


            fd = open(tokens[output_token_location], O_WRONLY|O_CREAT|O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execlp(tokens[0], tokens[0], NULL);
        perror("execlp");
        exit(-1);
    }

}

int execute_binary(char *tokens[], int tokenSize)
{

    pid_t pid;

    pid = fork();

    if(pid<0)
    {
        perror("fork()");
        exit(-1);
    }

    if(pid>0)
    {
        //parent


        wait(NULL);
        return 0;



    }
    else if(pid==0)
    {
        execvp(tokens[0], tokens);
        perror("execvp");
        exit(-1);

    }
}

int execute_one_pipe(char *tokens[], int pipe_location, int token_size)
{
    char *first_binary[pipe_location];
    char *second_binary[token_size-1-pipe_location];
    char *tempString;

    int index;
    pid_t pid;
    int fd[2];


    for(index = 0;index<pipe_location;++index){
        tempString = strdup(tokens[index]);

        strcat(tempString, "\0");
        //printf("%s\n", tempString);
        //strcpy(first_binary[index], tokens[index]);
        first_binary[index] = tempString;
        //printf(tempString);
        //printf("%s\n", first_binary[index]);
    }
    first_binary[pipe_location] = NULL;
    printf("index after first loop is %d. Pipe location is %d\n", index, pipe_location);

    int second_index=0;
    for(index = pipe_location+1; index < token_size; index++){

        tempString = strdup(tokens[index]);
        printf("in second loop %s\n", tempString);
        strcat(tempString, "\0");
        second_binary[second_index] = tempString;
        second_index++;
        //printf("%s\n", second_binary[index]);
    }
    second_binary[second_index] = NULL;



    //first_binary[0] = "ps";
    //second_binary[0] = "sort";


    wait(50);
    if(pipe(fd)<0){
        perror("error with pipe");
        exit(1);
    }


    pid = fork();
    if(pid < 0){
     perror("fork error");
     exit(1);
    }

    if(pid > 0){//parent

        close(fd[1]);
        if(dup2(fd[0],STDIN_FILENO)<0){
            perror("dup2 error");
            exit(1);
        }
        close(fd[0]);

        execvp(second_binary[0], second_binary);

        perror("second");
        exit(1);

    }else{
        printf("in child");
        close(fd[0]);
        if(dup2(fd[1], STDOUT_FILENO)<0){
            perror("dup2 error");
            exit(1);
        }
        close(fd[1]);

        execvp(first_binary[0], first_binary);

        perror("first");
        exit(1);
    }

}

int execute_two_pipes()
{
}

int execute_three_pipes()
{
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
    int input_token_location;
    int output_token_location;

    int number_of_pipes;
    int first_pipe_location;
    int second_pipe_location;
    int third_pipe_location;

    username = getlogin();
    //execution loop
    while(1)
    {
        IN_exists = 0;
        OUT_exists = 0;
        PIPE_exists = 0;
        number_of_pipes = 0;
        input_token_location = 0;
        output_token_location = 0;


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
            printf("in exit if\n");
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

        else//must be a shell command
        {

            if(n != 0)
            {
                index=0;//reset token index
                while(tokens[index]!=NULL)
                {

                    if(strcmp(tokens[index], OUT) == 0)
                    {
                        OUT_exists = 1;
                        output_token_location = index + 1;
                    }
                    else if(strcmp(tokens[index], IN) == 0)
                    {
                        IN_exists = 1;
                        input_token_location = index + 1;

                    }
                    else if(strcmp(tokens[index], PIPE) == 0 )
                    {
                        PIPE_exists = 1;
                        number_of_pipes++;

                        if(number_of_pipes == 1)
                        {
                            first_pipe_location = index;
                        }
                        else if(number_of_pipes == 2)
                        {
                            second_pipe_location = index;
                        }
                        else if(number_of_pipes == 3)
                        {
                            third_pipe_location == index;
                        }
                        else
                        {
                            perror("Number of pipes 0 or exceded");
                        }
                    }
                    index++;
                }

                if(OUT_exists == 1 || IN_exists == 1 && PIPE_exists == 0)
                {

                    handle_io(tokens, IN_exists, OUT_exists,input_token_location, output_token_location);
                }
                else if(PIPE_exists == 1)
                {
                    if(number_of_pipes == 1)
                    {
                        pid_t pid;
                        pid = fork();

                        if(pid < 0){
                            perror("fork error");
                        }

                        if(pid>0){
                            wait(NULL);
                        }else{
                        execute_one_pipe(tokens, first_pipe_location, index);
                        }
                    }
                    else if(number_of_pipes == 2)
                    {
                        //execute_two_pipes();
                    }
                    else if(number_of_pipes ==3)
                    {
                        //execute_three_pipes();
                    }
                    else
                    {
                        perror("pipe error");
                    }
                }
                else
                {
                    execute_binary(tokens, index);
                }
            }
        }

    }//endwhile
    return 0;
}
