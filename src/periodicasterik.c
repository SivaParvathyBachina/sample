#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

static void myhandler(int s) {

if(s == SIGALRM)
{
fprintf(stderr, "Master Time Done\n");
}

if(s == SIGINT)
{
fprintf(stderr, "Caught Ctrl + C Signal \n");
fprintf(stderr, "Killing the existing slaves in the system \n");
}
exit(1);
}

/*
static int setupinterrupt(void) {
struct sigaction act;
act.sa_handler = myhandler;
act.sa_flags = 0;
return sigaction(SIGPROF, &act, NULL);
}*/

static int setupinterruptforSIGINT(void) {
struct sigaction action;
action.sa_handler = myhandler;
action.sa_flags = 0;
return sigaction(SIGINT, &action, NULL);
}


/*
static int setupitimer(void) {
struct itimerval value;
value.it_interval.tv_sec = 2;
value.it_interval.tv_usec = 0;
value.it_value = value.it_interval;
return (setitimer(ITIMER_PROF, &value, NULL));
}
*/

int main(void) {
/*if (setupinterrupt() == -1) {
perror("Failed to set up handler for SIGPROF");
return 1;
} */
/*
if (setupinterruptforSIGINT() == -1) {
perror("Failed to set up handler for SIGINT");
return 1;
}*/

/*if (setupitimer() == -1) {
perror("Failed to set up the ITIMER_PROF interval timer");
return 1;
}*/
signal(SIGALRM, myhandler);
signal(SIGINT, myhandler);
alarm(2);
for ( ; ; );
} 



