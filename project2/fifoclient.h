#ifndef _FIFOCLIENT_H_
#define _FIFOCLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

// Modify maximum file size here if needed
// ---------------------------------------
#define MAX_FILE_SIZE 1024
// ---------------------------------------


void controlled_shutdown(int signal){
    printf("\nCaught signal %d",signal);
    printf("\nPerforming controlled shutdown...\n");
    exit(0);
}



#endif