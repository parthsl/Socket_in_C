#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#ifndef calle
#define calle(x,o,r,i,s,l,u,d) l##o##u##d
#endif
#define parth f##o##r##k

const int N = 10;
pid_t pids[10];

void sighandler( int sig){
	printf("Exited Signal received->%d\n\n\n\n\nThis code is presented by 16IS15F\n",sig);
	exit(1);
}
