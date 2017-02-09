#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <ctype.h>
#include <netdb.h>

int main(int argc, char **argv)
{
	int sockfd,n;
	char buffer[1024];
	struct sockaddr_in server_addr;
	socklen_t addr_size;
	struct hostent *server;
	FILE *fp;
	
	sockfd = socket(PF_INET, SOCK_DGRAM,0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( atoi(argv[2]));
	server = gethostbyname(argv[2]);
	bcopy( (char*)server->h_addr, (char*)&server_addr, sizeof(server_addr));
	memset(server_addr.sin_zero, '\0',sizeof(server_addr.sin_zero));
	addr_size = sizeof(server_addr);
	
	printf("Enter message:");
	fgets(buffer,1024,stdin);
	printf("You typed: %s\n",buffer);
	n = strlen(buffer)+1;
	if(sendto(sockfd,buffer,n,0,(struct sockaddr*)&server_addr,addr_size)==-1)
		printf("Error");
	while(recvfrom(sockfd,buffer,1024,0,NULL,NULL)>0){
		if(strncmp(buffer,"##end##",7)==0)break;
			printf("%s",buffer);
	}
	
	printf("Enter filename:");
	fgets(buffer,1024,stdin);
	printf("You selected: %s\n",buffer);
	n = strlen(buffer)+1;
	sendto(sockfd,buffer,n,0,(struct sockaddr*)&server_addr,addr_size);
	fp = fopen(buffer,"w");
	while(recvfrom(sockfd,buffer,1024,0,NULL,NULL)>0){
		if(strncmp(buffer,"##end##",7)==0)break;
			fprintf(fp,"%s",buffer);
	}
	fclose(fp);
	printf("File successfully transferred.....\n");
	return 0;
}

