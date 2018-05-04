#include "cleaning.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>



void controlled_shutdown(int signal){
    printf("\nCaught signal %d",signal);
    printf("\nPerforming controlled shutdown.\n");
    exit(0);
}


int main(int argc, char **argv){
    
    
    FILE *logfile;
    int status, fd;
    struct flock lock;
    
    pid_t pid;
   


    if (argc == 1) {
		printf("This program cleans comments and blank lines from given files.\n\nTo use, type on terminal: %s filename1 filename2 ...\n",argv[0]);
		return 0;
	} else {    
        // Creating a logfile
       
        char logname[48];
        time_t ltime;
        ltime=time(NULL); // get current calendar time
        
        strcpy(logname,asctime(localtime(&ltime))); // adds timestamp to filename
        strcat(logname,"_cleaning.log");
        
        logfile = fopen(logname, "w");

        if(logfile == NULL){
            printf("\nError, log file couldn't be created.\n");
            exit(1);
        }
    }
    
    
    signal(SIGINT, controlled_shutdown); // Checks for signals  
    
     
    for(int i=1; i<argc;i++) {

        char* file = argv[i];
        pid = fork();

        if(pid == -1) {
            perror("Error, fork failed\n");
            return -1;
            fputs("Error, Fork failed\n", logfile);
        }

        if(pid ==  0) {
		    // child process
            fputs("Child process alive\n",logfile);


            fd=open(file,O_WRONLY);
            memset (&lock, 0, sizeof(lock));
            lock.l_type = F_WRLCK;
            //Placing write lock.
            fcntl(fd, F_SETLKW, &lock);
            fprintf(logfile, "%s is now locked, proceeding to clean\n", argv[i]);
        

            if(!cleaning(argv[i])){
                fputs("Cleaning successful\n", logfile);
                fputs("Unlocking..\n", logfile);
                //unlock
                lock.l_type = F_UNLCK;
                fcntl (fd, F_SETLKW, &lock);
                close(fd);
                fprintf(logfile, "%s unlocked and closed\n", argv[i]);
            } else {
                close(fd);
                fputs("Cleaning failed. File closed\n", logfile);
            }
            
            exit(0);

	    } else {
		    // parent process
            fputs("Parent process alive\n", logfile);
	    }
    }


    //Parent waits for child processess to exit
    while(wait(&status) > 0) {
        fprintf(logfile, "Child process has exit with status %d\n", status);
    }

	
    fputs("Closing logfile and exiting...", logfile);
    fclose(logfile);
    
	return 0;
    
}