#include <stdio.h>
#include <stdlib.h>
#include "/home/patrick/School/cs3305/projects/cs3305shell/token.h"

#define HISTORY_SIZE 10
#define HISTORY_LINE_LENGTH 80
#define HISTORY_FILE_NAME "command_history.txt"

int file_exists(const char *fname){
	FILE *file;
	if(file = fopen(fname, "r")){
		fclose(file);
		return 1;
	}
	return 0;
}

int main()
{
    char input_line[MAX], *tokens[CMD_MAX];
    int i, n;
    int index;
    int historyIndex = 0;
    char line[HISTORY_LINE_LENGTH];
    char *historyString;
    char *historyArray[HISTORY_SIZE];
    
    FILE *historyFile;
    
    
	for(index=0;index<HISTORY_SIZE;index++){
		historyArray[index] = NULL;
	}
	
	//execution loop
	while(1){
		printf("patrick> ");

	    if(fgets(input_line, MAX, stdin) != NULL)
	    {
			if((historyString = strdup(input_line))!=NULL){
				
				//free(historyArray[historyIndex]);
				historyArray[historyIndex] = historyString;
				historyIndex++;
				
				if(historyIndex == HISTORY_SIZE){
					historyFile = fopen(HISTORY_FILE_NAME, "a");
					for(index=0;index<HISTORY_SIZE;index++){

						fprintf(historyFile, "%s", historyArray[index]);
						
					}
					
					fclose(historyFile);
					
					for(index=0;index<HISTORY_SIZE;index++){
						historyArray[index] = NULL;
					}
					
					historyIndex = 0;
				}
		}
	        n = make_tokenlist(input_line, tokens);
	        

	    } else {
	        printf("huh?\n");
	    }

	    for(i = 0; i<n; i++)
	    {
			
			if(strncmp(tokens[0], "exit", 4) == 0 && n==1){
				return 0;
			
			}else if(strncmp(tokens[0], "exit", 4) == 0 && n!=1){
				printf("'exit' takes 0 parameters\n");
			
			}else if(strncmp(tokens[0], "history", 7) == 0 && n==1){
				if(file_exists(HISTORY_FILE_NAME)){
					historyFile = fopen(HISTORY_FILE_NAME, "rt");
					while(fgets(line, HISTORY_LINE_LENGTH, historyFile)!=NULL)
					{
						printf("%s", line);
					}
				}
				
				
				for(index=0; index<HISTORY_SIZE; index++){
					if(historyArray[index] !=NULL){
						printf("%s", historyArray[index]);
					}
				}

			}else if(strncmp(tokens[0], "history", 7) == 0 && n!=1){
				printf("history take 0 parameters\n");
				break;
			}
	    }
	    
	}
}
