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

#define SHMSIZE 100

void create_child_processes(int);
void signalHandler(int);

pid_t childpid = 0;
int i, n,x,k,s, status;
//opterr = 0;

key_t myshmKey;
int shmId;
shared_mem *shmPtr;     


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
        fprintf(stderr, "Usage: %s -n processes -h [help] -s children to exist in the system at the same time \n", argv[0]);
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

//struct sigaction psa;
//psa.sa_handler = signalHandler;
//signal(SIGINT, signalHandler);
//sigaction(SIGINT, &psa, NULL);
//alarm(2);


myshmKey = ftok(".", 'c');
shmId = shmget(myshmKey, sizeof(shared_mem), IPC_CREAT | 0666);

if(shmId <0 )
{
	fprintf(stderr, "Error in shmget \n");
	exit(1);
}

shmPtr = (shared_mem*) shmat(shmId, NULL, 0);
//fprintf(stderr, "Shared Memory id in master %d \n", shmId);


if(shmPtr == (void *) -1)
{
	perror("Error in attaching shared memory --- Master \n");
	exit(1);
}

//fprintf(stderr, "Allocated shared memory \n");

shmPtr -> seconds = 0;

//fprintf(stderr, "After seconds \n");
shmPtr -> milliseconds = 0;

//fprintf(stderr, "%d, %d \n" ,shmPtr -> seconds, shmPtr -> milliseconds);

create_child_processes(n);

//fprintf(stderr, "Master executed succesfully");

//shmdt((void *)shmPtr);
//fprintf(stderr, "Server has detached its shared memory \n");
//shmctl(shmId, IPC_RMID, NULL);
//fprintf(stderr, "Server has removed its shared memory \n");



return 0;
}


void create_child_processes(int childCount)
{

	//fprintf(stderr, "%d Children to be spawn \n", childCount);
	//fprintf(stderr, " s count here %d \n ", s);
	pid_t pids[childCount];
	int kval = childCount;
	int j;
	while(j< childCount)	
	{
	//fprintf(stderr, "J entered %d \n", j-s);
	for(i=0; i<s ;i++)
	{
		if(j == childCount)
		break;
		//fprintf(stderr, "Starting child forking for child %d \n" ,i);
		pids[i] = fork();
		if(childpid < 0)
		perror("Error in forking");
		
		if(pids[i] == 0)
		{
		//fprintf(stderr, "Shared Memory id in create_child_processes %d \n", shmId);
		fprintf(stderr, "Child Forked %d \n" , j);
		char command1[50], command2[50], command3[50]; // Added
		char *i_val = "-i";
		char *s_val = "-s";
		char *k_val = "-n";
		char *i = i;
		//char *n = n;
		char *shmid;
		// fprintf(stderr, " n count here %d \n ", *n);
		char *temp[] = {NULL,i_val,command1,k_val,command2 ,s_val, command3, NULL}; // Modified
		temp[0]="./worker";
		sprintf(temp[2],"%d", i);
		sprintf(temp[4], "%d", n);
		sprintf(temp[6], "%d", shmId);
		execv("./worker", temp);
		fprintf(stderr, "Error in exec");
		}

		fprintf(stderr, "After Update Of Shared Memory %d, %d \n", shmPtr -> seconds, shmPtr -> milliseconds);
		/*else
		{
		waitpid(pids[i], &status, 0);
		fprintf(stderr, "waiting for child %ld to get finished in master \n", pids[i]);
		} */
	j++;
	}
	//for(i=0; i<s; i++) 
             //{
                waitpid(-1, &status, 0);
		sleep(2);
                //fprintf(stderr, "waiting for child %ld to get finished in master \n", pids[i]);
            // }		
	if(i >= s)
	{
		fprintf(stderr, "S is done these many times \n");
		i = 0;	
	}
	}
fprintf(stderr, "Master executed succesfully \n");

}


void signalHandler(int signalValue)
{
	//signal(SIGINT, SIG_IGN);
	//signal(SIGALRM, SIG_IGN);
	if(signalValue == SIGALRM)
	fprintf(stderr, "Signal Caught \n");

	if(signalValue == SIGINT)
	fprintf(stderr, "Received Ctrl + C signal");
} 
