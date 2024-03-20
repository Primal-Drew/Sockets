#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT "3490"
#define MAXDATASIZE 100

// Get sock address IPv4 or IPv6

void* get_in_addr(struct sockaddr *sa)
{
    if(sa->sa_family == AF_INET)
        return &(((sockaddr_in *)sa)->sin_addr);
    
    return &(((sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc,char* argv[])
{
    // Initialize variables that will be used
    int sockfd; /* File Descriptor */
    int numbytes;
    char buf[MAXDATASIZE];

    struct addrinfo hints,*servinfo,*p;
    int rv; /*Stores value returned by the getaddrinfo() function*/
    char s[INET6_ADDRSTRLEN];/*Stores ntop()*/

    // Error handling incase client does not provide the client hostname.
    if(argc !=2)
    {
        fprintf(stderr,"Usage: Client hostname");
        exit(1);
    }

    // Filling the @hints structs.

    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;


    if((rv= getaddrinfo(argv[1],PORT,&hints,&servinfo) ) != 0)
    {
        fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(rv));
        return 1;
    }

    for(p = servinfo;p != NULL;p = p->ai_next)
    {
        if((sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1)
        {
            perror("Client: socket");
             continue;
        }
       
        if((connect(sockfd,p->ai_addr,p->ai_addrlen))== -1)
        {
        close(sockfd);
        perror("Client: connect");
        continue;
        }
        break;
    }

    if(p == NULL)
    {
        fprintf(stderr,"Client: Failed to connect");
        return 2;
    }

    inet_ntop(p->ai_family,get_in_addr((struct sockaddr*)p->ai_addr),s,sizeof(s));
    printf("Connecting to: %s\n",s);

    freeaddrinfo(servinfo);

    if((numbytes=recv(sockfd,buf,MAXDATASIZE-1,0))==-1)
    {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
    printf("Client recieved: %s\n",buf);
    close(sockfd);
    return 0;
}