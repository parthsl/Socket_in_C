/*  Start with the usual includes and declarations.  */
#include "Daytime.h"

int main(int argc,char** argv)
{
    extern char *host;
    extern int sockfd;
    extern int len, result;
    extern struct sockaddr_in address;
    extern struct hostent *hostinfo;
    extern struct servent *servinfo;
    extern char buffer[128];

    if(argc == 1)
        host = "localhost";
    else
        host = argv[1];
        
    check();
    if(!hostinfo) {
        fprintf(stderr, "no host: %s\n", host);
        exit(1);
    }

/*  Check that the daytime service exists on the host.  */

    servinfo = getservbyname("daytime", "tcp");
    if(!servinfo) {
        fprintf(stderr,"no daytime service\n");
        exit(1);
    }
    printf("daytime port is %d\n", ntohs(servinfo -> s_port));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

/*  Construct the address for use with connect...  */

    address.sin_family = AF_INET;
    address.sin_port = servinfo -> s_port;
    bcopy( (char*)hostinfo->h_addr, (char*)&address.sin_addr.s_addr, hostinfo->h_length);
    len = sizeof(address);

/*  ...then connect and get the information.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);
    if(result == -1) {
        perror("oops: getdate");
        exit(1);
    }

    result = read(sockfd, buffer, sizeof(buffer));
    buffer[result] = '\0';
    printf("read %d bytes: %s", result, buffer);

    close(sockfd);
    exit(0);
}
