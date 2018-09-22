#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>

#include "shared_mem.h"

shared_mem *shmPtr;
int shmId,x, processNumber;

int main (int argc, char *argv[]) {


fprintf(stderr, "Worker Runnig \n");

int i;
for(i=0;i<argc;i++) 
       fprintf(stderr, "\nargv[%d]: %s",i,argv[i]); 

while((x = getopt(argc,argv, "i:s:")) != -1)
switch(x)
{
case 'i':
       // fprintf(stderr, "Procees Number  \n");
	processNumber = atoi(optarg);
        break;
case 's':
        shmId = atoi(optarg);
        break;
case '?':
        fprintf(stderr, "Invalid Arguments \n");
        return 1;
}

fprintf(stderr," Shared Memory Id in worker %d \n", shmId);

shmPtr = (shared_mem*) shmat(shmId, NULL, 0);

if(shmPtr == (void *) -1)
{
        perror("Error in attaching shared memory \n");
        exit(1);
}

fprintf(stderr, "Allocated shared memory \n");

shmPtr -> seconds += 20;

//fprintf(stderr, "After seconds \n");
shmPtr -> nanoseconds += 30;
fprintf(stderr, "%d, %d \n" ,shmPtr -> seconds, shmPtr -> nanoseconds);


return 0;
}

