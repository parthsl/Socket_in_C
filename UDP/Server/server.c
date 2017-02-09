#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <ctype.h>

#include <netdb.h>

const int BSize = 1024;
int main(int argc, char **argv)
{
	int sockfd;
	char buffer[1024];
	struct sockaddr_in server_addr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	struct hostent *server;
	FILE *fp;
	
	sockfd = socket(PF_INET, SOCK_DGRAM ,0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[1]));
	server = gethostbyname("localhost");
	bcopy((char*)server->h_addr, (char*)&server_addr.sin_addr.s_addr, sizeof(server_addr));
	memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
	
	bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	addr_size = sizeof(serverStorage);
	while(1){
		printf("Waiting for Client...\n");
		recvfrom(sockfd,buffer,1024,0,(struct sockaddr*)&serverStorage,&addr_size);
		//buffer[strlen(buffer)-2]='\0';
		printf("Obtained command:%s\n",buffer);
		fp = popen(buffer,"r");
		while(fgets(buffer,1024,fp)){
			printf("%s",buffer);
			sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&serverStorage,addr_size);
		}
		//closing connection by character stuffing
		bzero(buffer,1024);
		strcpy(buffer,"##end##");
		if(sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&serverStorage,addr_size)==-1)
			printf("Error");
		fclose(fp);
		//command executed
		
		//now recv filename and send file data to client
		recvfrom(sockfd,buffer,1024,0,(struct sockaddr*)&serverStorage,&addr_size);
		printf("Obtained filename:%s\n",buffer);
		buffer[strlen(buffer)-1]='\0';
		fp = fopen(buffer,"r");
		while(fgets(buffer,1024,fp))
			sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&serverStorage,addr_size);
		bzero(buffer,1024);
		strcpy(buffer,"##end##");
		if(sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&serverStorage,addr_size)==-1)
			printf("Error");
		fclose(fp);	
		printf("File sent...\nConnection with client closed...\n");
		
	}
	return 0;
}

