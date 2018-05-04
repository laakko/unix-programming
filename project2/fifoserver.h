#ifndef _FIFOSERVER_H_
#define _FIFOSERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>


struct thread_args{
    int i;
};


#endif