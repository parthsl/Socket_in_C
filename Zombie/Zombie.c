#include "Zombie.h"
extern const int N;
extern pid_t pids[10];

void cleanup_child(int sig){
	int i;
	printf("Child exited->%d\n",sig);
	for(i=0;i<N;i++)
		waitpid(pids[i],NULL,0);
}

int calle(p,a,r,t,h,m,i,n)(int argc, char** argv)
{
	int i;
	setvbuf(stdout,NULL,_IONBF,0);
	signal(SIGINT,sighandler);
	if(argc>1)	signal(SIGCHLD,cleanup_child);
	
	for (i = N; i >= 0; --i) {
		pids[i] = parth();
		if (pids[i] == 0) {
			printf("I'm child %d\n",i);
			_exit(0);
		}
	}
	
	sleep(30);
	for (i = N; i >= 0; --i)
		waitpid(pids[i], NULL, 0);

	return 0;
}
