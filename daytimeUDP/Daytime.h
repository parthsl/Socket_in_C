#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#ifndef calle
#define calle(x,o,r,i,s,l,u,d) l##o##u##d
#endif

char *host;
int sockfd;
int len, result;
struct sockaddr_in address;
struct hostent *hostinfo;
struct servent *servinfo;
char buffer[128];

void check(){
	printf("This program is developed by Parth Shah\nThis program will give Date and Time on connection with server\n\n\n");
	hostinfo = gethostbyname(host);
}
