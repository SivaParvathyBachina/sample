#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <signal.h>
#include "shared_mem.h"

#define MILLION 1000000

shared_mem *shmPtr;
int shmId,x, processNumber,n;

void signalHandler(int);
pid_t childpid;

int main (int argc, char *argv[]) {


//fprintf(stderr, "Worker Runnig \n");

//int i;
//for(i=0;i<argc;i++) 
 //      fprintf(stderr, "\nargv[%d]: %s",i,argv[i]); 

childpid = getpid();
fprintf(stderr, "Child From worker Id %ld \n", childpid);

while((x = getopt(argc,argv, "i:n:s:")) != -1)
switch(x)
{
case 'i':
       // fprintf(stderr, "Procees Number  \n");
	processNumber = atoi(optarg);
        break;

case 'n': 
	n = atoi(optarg);
	break;

case 's':
        shmId = atoi(optarg);
        break;
case '?':
        fprintf(stderr, "Invalid Arguments \n");
        return 1;
}

//fprintf(stderr," Shared Memory Id in worker %d \n", shmId);
alarm(2);
//signal(SIGINT, signalHandler);


shmPtr = (shared_mem*) shmat(shmId, NULL, 0);

if(shmPtr == (void *) -1)
{
        perror("Error in attaching shared memory \n");
        exit(1);
}

//fprintf(stderr, "K value %d \n", n);
//fprintf(stderr, "Allocated shared memory in worker \n");

int j,seconds = 0, milliseconds = 0; 

int loopCount = n*MILLION;

for(j=0; j< loopCount; j++)
{
	if(milliseconds == 999)
	{
		seconds = seconds + 1;
		milliseconds = 0;
	}
	milliseconds++;
}

shmPtr -> seconds = seconds;
shmPtr -> milliseconds = milliseconds;

fprintf(stderr, "After Update in worker  ");
fprintf(stderr, "%d, %d \n" ,shmPtr -> seconds, shmPtr -> milliseconds);

//fprintf(stderr, "Killing child with %ld \n", childpid);


//kill(childpid, SIGINT);
fprintf(stderr, "Child Exiting from worker with %ld \n", childpid);

return 0;
}


void signalHandler(int signalValue)
{
        fprintf(stderr, "Inside Signal Handler \n");
        if(signalValue == SIGALRM)
	        fprintf(stderr, "Signal Caught \n");
	if(signalValue == SIGINT)
        {
           fprintf(stderr, "Inside Signal Handler \n");
	   fprintf(stderr, "Received Ctrl + C signal");
	}    
 }


