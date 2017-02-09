#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>

enum modes {text,getfile,changemode,command};

void error(char *msg){
  perror(msg);
  exit(1);
}

void writeToSocket(int sockfd, char* buffer){
  write(sockfd, buffer, 1024);
  buffer[strlen(buffer)-1]='\0';
}

int main(int argc, char* argv[]){
  int sockfd,portno,n;
  struct sockaddr_in server_addr;
  struct hostent * server;
  char buffer[1024];
  enum modes mode = text;
  FILE* fp;

  if(argc<3){
    fprintf(stderr,"%s\n","Not enough arguments!!!!");
    exit(1);
  }
  portno = atoi(argv[2]);
  sockfd = socket( AF_INET, SOCK_STREAM, 0);
  server = gethostbyname(argv[1]);
  if(server==NULL){
    error("No host specified!!!");
  }
  bzero( (char*)&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  bcopy( (char*) server->h_addr,
          (char*)&server_addr.sin_addr.s_addr,
          server->h_length);
  server_addr.sin_port = htons(portno);
  if( connect( sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr) )<0 ){
    error("Error connecting!!!!");
  }
  do{
    printf("$>");
    bzero(buffer,1024);
    fgets(buffer,1024,stdin);
    if(strncmp(buffer,"cm",2)==0){
      if(strncmp(buffer,"cm-text",4)==0)mode=text;
      else if(strncmp(buffer,"cm-cmd",6)==0)mode=command;
      else if(strncmp(buffer,"cm-file",7)==0){mode=getfile;printf("Enter filename:\n");}
      writeToSocket(sockfd,buffer);
      printf("Mode changed to %d:%s\n",mode,buffer );
      continue;
    }
    else if(mode==text){
      writeToSocket(sockfd,buffer);
    }
    else if(mode==command){
      printf("In Command Mode\n");
      writeToSocket(sockfd,buffer);
      do {
        bzero(buffer,1024);
        n = read(sockfd,buffer,1024);
        if(strncmp(buffer,"##end##",7)==0)break;
        fprintf(stderr, "%s\n", buffer);
      } while(n>0);
    }
    //file sending code
    else if(mode==getfile){
      fp = fopen(buffer,"w");
      writeToSocket(sockfd,buffer);
      do{
        n = read(sockfd,buffer,1024);
        if(strncmp(buffer,"##end##",7)==0)break;
        fprintf(fp,"%s",buffer);
      }while (n>0);
      fclose(fp);
      printf("Changing mode to text\n");
      mode=text;
    }

  }while (strcmp(buffer,"exit")!=0);
  close(sockfd);
  return 0;
}
