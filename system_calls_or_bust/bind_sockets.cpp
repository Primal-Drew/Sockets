#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

int main()
{
    struct addrinfo hints,*res;
    memset(&hints,0,sizeof(hints));
    int status;
    int socketfd;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((status=getaddrinfo(NULL,"3490",&hints,&res))!= 0)
    {
        fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(status));
    }

    socketfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

    bind(socketfd,res->ai_addr,res->ai_addrlen);
    
}
