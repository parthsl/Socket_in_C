#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

enum modes {command,text,changemode,sendfile};

void error(char *msg){
  perror(msg);
  exit(1);
}

void shellExec(int newsockfd, char* buffer){
  char line[1024];
  FILE* fp;
  fp = popen(buffer,"r");
  while(fgets(line,1024,fp)){
    printf("%s\n", line);
    write(newsockfd,line,1024);
  }
  write(newsockfd,"##end##",1024);
  printf("done\n");
  pclose(fp);
}

void readDatafromSocket(int newsockfd, char* buffer){
  bzero(buffer,1024);
  read(newsockfd,buffer,1023);
  buffer[strlen(buffer)-1]='\0';
}

void sendFileOverSocket(int sockfd,char* filename, char* path){
  printf("sendFileOverSocket : %s\n", filename);
  FILE* fp = fopen(filename,"r");
  char buffer[1024];
  while(fgets(buffer,1024,fp)){
    printf("Writing file: %s",buffer);
    write(sockfd,buffer,1024);
  }
  write(sockfd,"##end##",1024);
  fclose(fp);
  printf("Closing File : %s\n", filename);
}


int main(int argc, char* argv[]){
  int sockfd,newsockfd,portno;
  struct sockaddr_in server_addr,client_addr;
  char buffer[1024];
  socklen_t clilen;
  enum modes mode = text;
  //set fflush everytime
  setbuf(stdout,NULL);
  setbuf(stdin,NULL);

  if(argc<2){
    fprintf(stderr, "%s\n","Provide Port number also!!!!!");
    exit(1);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  portno = atoi(argv[1]);
  bzero( (char*)&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(portno);
  if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
    error("Binding to socket failed!!!!");
  }
  listen(sockfd,5);
  clilen = sizeof(client_addr);
  newsockfd = accept(sockfd,(struct sockaddr*)&client_addr, &clilen);

  //loop untill exit comman typed
  do {
    readDatafromSocket(newsockfd,buffer);
    if(strcmp(buffer,"\0")==0)continue;
    if(strncmp(buffer,"cm",2)==0){
		if(strcmp(buffer,"cm-file")==0)
			mode=sendfile;
		else if(strcmp(buffer,"cm-cmd")==0) mode=command;
		else mode = text;
		continue;
    }
    else if(mode==command){
      printf("Executing shell commands\n");
      shellExec(newsockfd,buffer);
    }
    else if (mode==text)
      printf("Obtained text: %s\n", buffer);
    else if (mode==sendfile){
      printf("%s\n",buffer );
      sendFileOverSocket(newsockfd,buffer,NULL);
    }
  } while(strcmp(buffer,"exit")!=0);
    close(newsockfd);
    close(sockfd);
}
