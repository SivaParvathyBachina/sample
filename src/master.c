#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <sys/time.h>
#include <signal.h>
#include "shared_mem.h"

#define SHMSIZE 100


pid_t childpid = 0;
int i,k, n,x,k,s,j,status;
pid_t *child_pids;
key_t myshmKey;
int shmId;
shared_mem *clock;     

void clearSharedMemory() {
fprintf(stderr, "Master Clock Value Seconds %d, Milliseconds %d \n", clock -> seconds, clock -> milliseconds);
shmdt((void *)clock);
fprintf(stderr, "Master started detaching its shared memory \n");
shmctl(shmId, IPC_RMID, NULL);
fprintf(stderr, "Master removed shared memory \n");
}

void killExistingChildren(){

for(k=0; k<n; k++)
{
if(child_pids[k] != 0)
{
fprintf(stderr, "Killing child with Id %d \n", child_pids[k]);
kill(child_pids[k], SIGTERM);
}
}

}



void myhandler(int s) {
if(s == SIGALRM)
{
fprintf(stderr, "Master Time Done\n");
killExistingChildren();
clearSharedMemory();
}

if(s == SIGINT)
{
fprintf(stderr, "Caught Ctrl + C Signal \n");
fprintf(stderr, "Killing the Existing Children in the system \n");
killExistingChildren();
clearSharedMemory();
}
exit(1);
}

int main (int argc, char *argv[]) {

if (argc < 2){ // check for valid number of command line arguments
fprintf(stderr, "Invalid number of arguments. Please give it in the following format");
fprintf(stderr, "Usage: %s  -n processess -h [help] -p [error message]", argv[0]);
return 1;
}
while((x = getopt(argc,argv, "hn:s:")) != -1)
switch(x)
{
case 'h':
        fprintf(stderr, "Usage: %s -n processCount -h [help] -s childrenCount\n", argv[0]);
        return 1;
case 'n':
        n = atoi(optarg);
        break;
case 's':
	s = atoi(optarg);
	break;
case '?':
        fprintf(stderr, "Please give '-h' for help to see valid arguments \n");
        return 1;
}

if(s>n)
{
fprintf(stderr,  "Illegal COmmand Line Arguments..s cannot be greater  than n. \n", s ,n );
return 1;
}
if(n> 20)
{
fprintf(stderr, "There can only be a maximum of 20 processesin the system\n");
fprintf(stderr, "Setting back n to 20 \n");
n = 20;
}


signal(SIGINT, myhandler);	
signal(SIGALRM, myhandler);
alarm(2);

myshmKey = ftok(".", 'c');
shmId = shmget(myshmKey, sizeof(shared_mem), IPC_CREAT | 0666);

if(shmId <0 )
{
	fprintf(stderr, "Error in shmget \n");
	exit(1);
}

clock = (shared_mem*) shmat(shmId, NULL, 0);
if(clock == (void *) -1)
{
	perror("Error in attaching shared memory --- Master \n");
	exit(1);
}

clock -> seconds = 0;
clock -> milliseconds = 0;

int childCount = n;
child_pids = (pid_t *)malloc(n * sizeof(int));
	for(i=0; i<s ;i++)
	{
		child_pids[i] = fork();
		if(child_pids[i] == 0)
		{
		char argument1[50], argument2[50], argument3[50];
		char *s_val = "-s";
		char *k_val = "-n";
		char *shmid;
		char *arguments[] = {NULL,k_val,argument2 ,s_val, argument3, NULL};
		arguments[0]="./worker";
		sprintf(arguments[2], "%d", n);
		sprintf(arguments[4], "%d", shmId);
		execv("./worker", arguments);
		fprintf(stderr, "Error in exec");
		}	
	j++;
	}
	
	waitpid(-1, &status, 0);
	while(j < childCount)
	{
	//sleep(1);
         waitpid(-1, &status, 0);
	 child_pids[j] = fork();
         if(child_pids[j] == 0)
         {
	char ival[10], nval[50],sval[50];
         char *s_val2 = "-s";
         char *k_val2 = "-n";
         char *j = j;
	char *arguments2[] = {NULL,k_val2,nval ,s_val2, sval, NULL};
	 arguments2[0]="./worker";
         sprintf(arguments2[2], "%d", n);
         sprintf(arguments2[4], "%d", shmId);
         execv("./worker", arguments2);
         fprintf(stderr, "Error in exec");
	}
       	j++;
	}

while((waitpid(-1, &status, 0) > 0 )){};
clearSharedMemory();
return 0;
}

