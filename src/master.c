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
pid_t *pids;
key_t myshmKey;
int shmId;
shared_mem *shmPtr;     

void myhandler(int s) {

if(s == SIGALRM)
{
fprintf(stderr, "Master Time Done\n");
fprintf(stderr, "Master Clock Value Seconds %d, Milliseconds %d \n", shmPtr -> seconds, shmPtr -> milliseconds);
shmdt((void *)shmPtr);
fprintf(stderr, "Master started detaching its shared memory \n");
shmctl(shmId, IPC_RMID, NULL);
fprintf(stderr, "Master removed shared memory \n");
}

if(s == SIGINT)
{
fprintf(stderr, "Caught Ctrl + C Signal \n");
fprintf(stderr, "Killing the existing slaves in the system \n");
for(k=0; k<n; k++)
{
if(pids[k] != 0)
{
fprintf(stderr, "Killing child with Id %d \n", pids[k]);
kill(pids[k], SIGTERM);
}
}
fprintf(stderr, "Master Clock Value Seconds %d, Milliseconds %d \n", shmPtr -> seconds, shmPtr -> milliseconds);
shmdt((void *)shmPtr);
fprintf(stderr, "Master started detaching its shared memory \n");
shmctl(shmId, IPC_RMID, NULL);
fprintf(stderr, "Master removed shared memory \n");
}
exit(1);
}

int setupinterrupt(void) {
struct sigaction act;
act.sa_handler = myhandler;
act.sa_flags = 0;
return sigaction(SIGPROF, &act, NULL);
}


int setupinterruptforSIGINT(void) {
struct sigaction action;
action.sa_handler = myhandler;
action.sa_flags = 0;
return sigaction(SIGINT, &action, NULL);
}

int setupitimer(void) {
struct itimerval value;
value.it_interval.tv_sec = 2;
value.it_interval.tv_usec = 0;
value.it_value = value.it_interval;
return (setitimer(ITIMER_PROF, &value, NULL));
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

if(s>n)
{
fprintf(stderr,  "Illegal COmmand Line Arguments..s cannot be  than n. \n", s ,n );
return 1;
}
if(n> 20)
{
fprintf(stderr, "There can only be a maximum of 20 processesin the system\n");
fprintf(stderr, "Setting back n to 20 \n");
n = 20;
}


if (setupitimer() == -1) {
perror("Failed to set up the ITIMER_PROF interval timer");
return 1;
}

if (setupinterrupt() == -1) {
perror("Failed to set up handler for SIGPROF");
return 1;
}

if (setupinterruptforSIGINT() == -1) {
perror("Failed to set up handler for SIGINT");
return 1;
}

signal(SIGALRM, myhandler);
alarm(2);

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

int childCount = n;
//fprintf(stderr, "Timer Set \n");
pids = (pid_t *)malloc(n * sizeof(int)); ;
//waitpid(-1, &status, 0);
	for(i=0; i<s ;i++)
	{
		pids[i] = fork();
		if(pids[i] == 0)
		{
		fprintf(stderr, "Child Forked %d \n" , j);
		char command1[50], command2[50], command3[50];
		//char *i_val = "-i";
		char *s_val = "-s";
		char *k_val = "-n";
		char *shmid;
		char *temp[] = {NULL,k_val,command2 ,s_val, command3, NULL};
		temp[0]="./worker";
		//sprintf(temp[2],"%d", i);
		sprintf(temp[2], "%d", n);
		sprintf(temp[4], "%d", shmId);
		execv("./worker", temp);
		fprintf(stderr, "Error in exec");
		}	
	j++;
	}
	waitpid(-1, &status, 0);
	while(j < childCount)
	{
	//sleep(1);
	fprintf(stderr, "Child Forked %d \n" , j);
         waitpid(-1, &status, 0);
	 pids[j] = fork();
         if(pids[j] == 0)
         {
	char ival[10], nval[50],sval[50];
         char *s_val2 = "-s";
         char *k_val2 = "-n";
         char *j = j;
	char *temp2[] = {NULL,k_val2,nval ,s_val2, sval, NULL};
	 temp2[0]="./worker";
         sprintf(temp2[2], "%d", n);
         sprintf(temp2[4], "%d", shmId);
         execv("./worker", temp2);
         fprintf(stderr, "Error in exec");
	}
       	j++;
	}

while((waitpid(-1, &status, 0) > 0 ))
{};
fprintf(stderr, "Master Clock Value Seconds %d, Milliseconds %d \n", shmPtr -> seconds, shmPtr -> milliseconds);
shmdt((void *)shmPtr);
fprintf(stderr, "Server has detached its shared memory \n");
shmctl(shmId, IPC_RMID, NULL);
fprintf(stderr, "Server has removed its shared memory \n");
return 0;
}

