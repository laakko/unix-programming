#include "fifoserver.h"
#define buffer_max 1024
#define MAX_NAME_LEN 20
#define MAX_THREADS 256

pthread_mutex_t mutex;
char *fifopipe = "/tmp/fifopipe";

 
static void *readwrite_newfile(void* ptr) {
// This function reads data from FIFO and copies it to newfile.

    struct thread_args *args = ptr;
    int i = args->i;
    
    char file_name[MAX_NAME_LEN];
    char buf[buffer_max];
    FILE *newfile, *logfile;

    int fd, fd1, j;
    free(args);

    for(j=0;;j++) {

        logfile = fopen("fifoserver_log.txt", "a");
        if(logfile == NULL){
            printf("\nError, log file couldn't be created.\n");
            exit(1);
        } 

    
        pthread_mutex_lock(&mutex); 

        fd = open(fifopipe, O_RDONLY);
        fd1 = dup(fd);
        close(fd);
        fputs("Server: fifopipe opened for reading.. \n", logfile);
        
        pthread_mutex_unlock(&mutex);

        int n = read(fd1,buf,buffer_max);
        close(fd1);
        
        if(n > 0) {
            fprintf(logfile, "Server: fifo buffer successfully read in thread #%d \n", i);
            sprintf(file_name, "newfile_%d_%d", i, j);
            newfile=fopen(file_name,"w");
            fwrite(buf, 1, n, newfile);
            fclose(newfile);
            fprintf(logfile, "Server: New file created: %s\n", file_name);
    
        } else {
             printf("Error reading received file");
             fprintf(logfile, "Server: Error reading received file in thread #%d \n", i);
        }

        fclose(logfile);
        
    }

    return NULL;
}



int main(void){
// This is a daemon program which creates threads
    

    FILE *logfile;
    time_t ltime;
    // get current calendar time
    ltime=time(NULL);
    
    logfile = fopen("fifoserver_log.txt", "a");
    if(logfile == NULL){
        printf("\nError, log file couldn't be created.\n");
        exit(1);
    } 
    
    
    fprintf(logfile, "\n\n%s Server: Starting up, initializing fifo and threads.. \n", asctime(localtime(&ltime)));
    mkfifo(fifopipe, 0666);
    int i;
    pthread_t tid[MAX_THREADS];


    // Initialize mutex for thread synchronization
    pthread_mutex_init(&mutex, NULL);

    // Daemonize
    daemon(1,1);
    fputs("Server: Daemonizing..\n", logfile);
  

    fputs("Server: Creating thread pool\n", logfile);
    fclose(logfile);
    
    // Thread pool
    for(i=0; i<MAX_THREADS; i++) {
        struct thread_args *argthread;
        argthread = malloc(sizeof(argthread));
        argthread->i = i;                                               
        pthread_create(&tid[i], NULL, readwrite_newfile, argthread); 
    }

    // Join with threads
    for(i=0; i<MAX_THREADS; i++) {
            pthread_join(tid[i], NULL);
    }
          
    // Remove fifo and mutex
    pthread_mutex_destroy(&mutex);
    unlink(fifopipe);

    return 0;
}