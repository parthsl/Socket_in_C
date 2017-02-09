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
     char temp[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;    
    
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
		 
			 memset((void*)buffer,0,100);
			 n = read(newsockfd,(void*)buffer,255);
			 if (n < 0) error("ERROR reading from socket");
			 printf("Received message: %s",buffer);
			 strcpy(temp,stringrev(buffer));
			 printf("Reverse string of the message:%s\n",temp);
			 n = write(newsockfd,temp,strlen(temp)+1);
			 if (n < 0) error("ERROR writing to socket");
	
		 close(newsockfd);
	 }
	 close(newsockfd);
     close(sockfd);
     return 0; 
}
