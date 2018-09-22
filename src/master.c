#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>

#include "shared_mem.h"

#define SHMSIZE 100

void create_child_processes(int);

pid_t childpid = 0;
int i, n,x,k,s;
//opterr = 0;

char *i_val, *shmid_val;
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

myshmKey = ftok(".", 'c');
shmId = shmget(myshmKey, sizeof(shared_mem), IPC_CREAT | 0666);

if(shmId <0 )
{
	fprintf(stderr, "Error in shmget \n");
	exit(1);
}

shmPtr = (shared_mem*) shmat(shmId, NULL, 0);
fprintf(stderr, "Shared Memory id in master %d \n", shmId);


if(shmPtr == (void *) -1)
{
	perror("Error in attaching shared memory --- Master \n");
	exit(1);
}

fprintf(stderr, "Allocated shared memory \n");

shmPtr -> seconds = 20;

//fprintf(stderr, "After seconds \n");
shmPtr -> nanoseconds = 30;

fprintf(stderr, "%d, %d \n" ,shmPtr -> seconds, shmPtr -> nanoseconds);

create_child_processes(s);


shmdt((void *)shmPtr);
fprintf(stderr, "Server has detached its shared memory \n");
shmctl(shmId, IPC_RMID, NULL);
fprintf(stderr, "Server has removed its shared memory \n");



return 0;
}


void create_child_processes(int childCount)
{
	for(i=0; i<childCount ;i++)
	{
		fprintf(stderr, "Starting child forking for child %d \n" ,i);
		childpid = fork();
		if(childpid < 0)
		perror("Error in forking");
		
		if(childpid == 0)
		{
		fprintf(stderr, "Shared Memory id in create_child_processes %d \n", shmId);
		fprintf(stderr, "Child Forked %d \n" , i);
		char command1[50], command2[50]; // Added
		char *i_val = "-i";
		char *s_val = "-s";
		char *i = i;
		char *shmid;
	//	strcpy(shmid, "%d", shmId);
		//strcat(i, i_val);
                //sprintf(shmid,"%s", s_val);
		char *temp[] = {NULL,i_val,command1,s_val, command2, NULL}; // Modified
		temp[0]="./worker";
		//strcat(i, i_val);
		//sprintf(shmid,"%s", s_val);
		//sprintf(temp[1],"%s","-i"); // remove *
		sprintf(temp[2],"%d", i); // remove *
		//sprintf(temp[3], "%s", "-s"); 
		sprintf(temp[4], "%d", shmId);
		//fprintf(stderr, "%s \n" , i_val);
		//fprintf(stderr, "%s \n", s_val);
		//fprintf(stderr, "Just before the exec call \n");
		execv("./worker", temp);
		fprintf(stderr, "Error in exec");
		}
		
	}
}
