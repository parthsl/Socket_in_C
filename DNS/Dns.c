#include "Dns.h"

char* fileSearch(char * site){
	FILE* fp = fopen("dns.txt","r");
	char buffer[1024];
	char * temp;
	while(fgets(buffer,1024,fp)){
		temp = strtok(buffer," ");
		if(strcmp(site,temp)==0){
			temp = strtok(NULL," ");
			return temp;
		}
	}
	fclose(fp);
	return NULL;
}

int calle(p,a,r,t,h,m,i,n)()
{
	struct hostent *server;
	struct in_addr **addr_list;
	int i;
	char buffer[1024];
	char *t;
	printf("Enter website name: ");
	scanf("%s",buffer);
	if((t=is15f(buffer))==NULL){
		server = gethostbyname(buffer);
		
		addr_list = (struct in_addr**)server->h_addr_list;
		printf("Official name of given website: %s\n",server->h_name);
		for (i=0;addr_list[i]!='\0';i++){
			printf("IP address from DNS server:%s\n",inet_ntoa(*addr_list[i]));
		}
	}
	else{
		printf("For website:%s\n",buffer);
		printf("IP address found from \'%s\':%s","dns.txt",t);
	}
	return 0;
}

