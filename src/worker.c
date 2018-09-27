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

shared_mem *clock;
int shmId,x, processNumber,n;

void signalHandler(int);
pid_t childpid;
int childCount = 0;
int main (int argc, char *argv[]) {

childpid = getpid();
fprintf(stderr, "Child Created \n Child started execution in  worker::: %ld \n", childpid);
fprintf(stderr, "*******************************************\n");

while((x = getopt(argc,argv, "n:s:")) != -1)
switch(x)
{
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


clock = (shared_mem*) shmat(shmId, NULL, 0);

if(clock == (void *) -1)
{
        perror("Error in attaching shared memory \n");
        exit(1);
}

int j;
int loopCount = n*MILLION;
for(j=0; j< loopCount; j++)
{
	if(clock -> milliseconds >= 999)
	{
		clock -> seconds  = clock -> seconds  + 1;
		clock -> milliseconds = 0;
	}
	clock -> milliseconds++;
}

fprintf(stderr, "Worker Clock Update ");
fprintf(stderr, " Seconds %d, Milliseconds %d \n" ,clock -> seconds, clock -> milliseconds);
fprintf(stderr, "Child Done Execution from worker with %ld \n", childpid);
fprintf(stderr, "------------------------------------------------ \n");
return 0;
}

