#include "cleaning.h"
#include <ctype.h>


int isempty_line(const char *line) {
// Checks for empty lines

    char c;
    while((c = *line++) != '\0') {
        if(!isspace(c)) {
            return 0;
        }
    }
    return 1;
}


int cleaning(const char *filename){
// This function does the cleaning and takes filename as an argument
    
    char line[2048]; 
    int comment = 0;
    FILE *input, *output;

    input = fopen(filename,"r");
    if(input == NULL){
        printf("\nError 404, file not found.\n");
        exit(1);       
    }
   
   
    char *newfilename = (char*)malloc(strlen(filename) + 7);
    
    strcpy(newfilename, filename);
    strcat(newfilename, ".clean");

    output = fopen(newfilename,"w");
    if(output == NULL){
        printf("\nError, file couldn't be created.\n");
        exit(1);
    }
  
    
    while(fgets(line, sizeof(line), input) !=NULL){
    // Removes comments from the lines of given file
   
        int i,j;
	    int comment2 = 0;
    
        for(i=j=0; line[j]; j++) {

	        if(line[j] == '\n') {
	            line[i++] = '\n'; 
            } else if(comment) {
                if(line[j] =='*' && line[j+1] && line[j+1] =='/') {
                    comment = 0; j++;
                } 
	        } else if(comment2) {
	        } else if(line[j]=='/' && line[j+1] && line[j+1]=='/') {  // Case of // coments
	            comment2 = 1; j++;
            } else if(line[j]=='/' && line[j+1] && line[j+1]=='*') {  // Case of  /* */ comments
	            comment = 1; j++;
            } else {
                line[i++]=line[j];    
            }                                                
        }


        line[i]='\0';

        // Doesn't copy empty lines into cleaned file
        if(!(isempty_line(line))) {
            fputs(line, output);
        } 
    } 

    
    char cwd[128];
    printf("\n%s succesfully cleaned into:\n%s/%s\n\n", filename, getcwd(cwd, sizeof(cwd)), newfilename);

    free(newfilename);
    fclose(input);
    fclose(output);
    return(0);
}


