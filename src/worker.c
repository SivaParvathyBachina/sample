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
 //     fprintf(stderr, "\nargv[%d]: %s",i,argv[i]); 

childpid = getpid();
fprintf(stderr, "Child started execution in  worker::: %ld \n", childpid);
fprintf(stderr, "*******************************************\n");

while((x = getopt(argc,argv, "n:s:")) != -1)
switch(x)
{
//case 'i':
  ;     // fprintf(stderr, "Procees Number  \n");
//	processNumber = atoi(optarg);
   //     break;

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
//alarm(2);
//signal(SIGINT, signalHandler);


shmPtr = (shared_mem*) shmat(shmId, NULL, 0);

if(shmPtr == (void *) -1)
{
        perror("Error in attaching shared memory \n");
        exit(1);
}

//fprintf(stderr, "K value %d \n", n);
//fprintf(stderr, "Allocated shared memory in worker \n");

int j;
//seconds = shmPtr -> seconds, milliseconds = shmPtr -> milliseconds; 

int loopCount = n*MILLION;

for(j=0; j< loopCount; j++)
{
	if(shmPtr -> milliseconds >= 999)
	{
		shmPtr -> seconds  = shmPtr -> seconds  + 1;
		shmPtr -> milliseconds = 0;
	}
	shmPtr -> milliseconds++;
}

//shmPtr -> seconds = seconds;
//shmPtr -> milliseconds = milliseconds;

fprintf(stderr, "Worker Clock Update ");
fprintf(stderr, " Seconds %d, Milliseconds %d \n" ,shmPtr -> seconds, shmPtr -> milliseconds);

//fprintf(stderr, "Killing child with %ld \n", childpid);


//kill(childpid, SIGINT);
fprintf(stderr, "Child Done Execution from worker with %ld \n", childpid);
fprintf(stderr, "=========================================== \n");

//shmdt((void *)shmPtr);
//shmctl(shmId, IPC_RMID, NULL);
//fprintf(stderr, "Clear Shared Memory \n");
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


