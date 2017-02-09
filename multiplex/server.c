/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <signal.h>
#include <ctype.h>

int sockfd, newsockfd;
void exitHandler(int sigint){
	printf("Exit Signal caught!!!!\n");
	close(newsockfd);
	close(sockfd);
}
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

char* stringrev(char* str){
	char temp;
	int i, j = 0;
	i = 0;
    j = strlen(str) - 1;
 
    while (i < j) {
       temp = str[i];
       str[i] = str[j];
       str[j] = temp;
       i++;
       j--;
    }
	return str;
}

int main(int argc, char *argv[])
{
     int portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     fd_set readfs;
     struct timeval timeout;
     int sret;
     
     signal(SIGINT,exitHandler);
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     if(argc<2)
		error("not enought arguments!!!!");
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     
     while(1==1){
		 newsockfd = accept(sockfd, 
					 (struct sockaddr *) &cli_addr, 
					 &clilen);
		 if (newsockfd < 0) 
			  error("ERROR on accept");
		 FD_ZERO(&readfs);
		 FD_SET(newsockfd,&readfs);
		 timeout.tv_sec = 7;
		 timeout.tv_usec = 0;
		 printf("waiting for select....\n");
		 sret = select(8,&readfs,NULL,NULL,&timeout);
		 if(sret==0){
			 printf("timeout...!!!\n");
			 sprintf(buffer,"timeout\n");
			 write(newsockfd,buffer,strlen(buffer));
		 }
		 else{
			 memset((void*)buffer,0,100);
			 n = read(newsockfd,(void*)buffer,255);
			 if (n < 0) error("ERROR reading from socket");
			 printf("Received message: %s",buffer);
			 for(n=0; buffer[n]!='\0';n++)
				buffer[n] = toupper(buffer[n]);
			 printf("Uppercase of message:%s\n",buffer);
			 n = write(newsockfd,buffer,255);
			 if (n < 0) error("ERROR writing to socket");
		 }
		 close(newsockfd);
	 }
	 close(newsockfd);
     close(sockfd);
     return 0; 
}
