#include "fifoclient.h"

int main(int argc, char **argv){
// This program is used to send file contents into fifopipe
    
    int f;
    FILE *file;
    FILE *logfile;
    char cwd[128];
    char content[MAX_FILE_SIZE];
    char *fifopipe = "/tmp/fifopipe";
 
    logfile = fopen("fifoserver_log.txt", "a");
    if(logfile == NULL){
        printf("\nError, log file couldn't be created.\n");
        exit(1);
    } 
   
    if(argc == 1) {
        printf("usage: ./fifowrite FILENAME1 \n");
    } else if(argc > 2) {
        printf("Too many arguments. Usage: ./fiforead FILENAME \n");
        fputs("Client: Too many input arguments. File not sent.\n", logfile);
        exit(0);
    } else {
    
        // Checks for signals  
        signal(SIGINT, controlled_shutdown); 
        

        // Open file for reading
        file = fopen(argv[1],"r");
        if(file == NULL){
            perror("error");
            fprintf(logfile, "Client: Error, cant open file %s.\n", argv[1]);
            exit(0);
            }

        fprintf(logfile, "Client: proceeding to send %s\n", argv[1]);

        // Find out size of file
        struct stat st;
        stat(argv[1], &st);
        int size = st.st_size;
        printf("size: %d", size);
        fprintf(logfile, "Client: Size of file acquired: %d\n", size);

        if(size > MAX_FILE_SIZE){
            printf("Couldn't send file to server: it's too large.\n Maximum file size for");
            printf("this program can be modified in %s/fiforeader.h \n", getcwd(cwd, sizeof(cwd)));
            fputs("Client: Couldn't send file to server, it's too large.\n", logfile);
            exit(0);
            
        }

        // Read contents of file 
        fputs("Client: Reading file contents...\n",logfile);
        fseek(file, SEEK_SET, 0);
        fread(content, size, 1, file);
        fclose(file);
       
        // Write to pipe
        fputs("Client: Writing to pipe...\n", logfile);
        f = open(fifopipe, O_WRONLY);
        write(f, content, size); 
        close(f);
        fputs("Client: Writing to pipe successful.\n", logfile);

    } 

    fputs("Client: Exiting..\n\n",logfile);
    fclose(logfile);        
    return 0;   
}
