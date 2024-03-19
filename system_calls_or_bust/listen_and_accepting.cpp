#include <string.h> 
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define BUFFER_SIZE 1024
int main()
{
    int sockfd,newfd;
    int status;
    char* msg;
    char buffer[BUFFER_SIZE];
    int len,byte_sent,byte_received;
    socklen_t addr_size;
    struct addrinfo hints,*res;
    struct sockaddr_storage their_addr;
    memcpy(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    if(status = getaddrinfo(NULL,"3490",&hints,&res) != 0)
    {
        fprintf(stderr,"getaddri: %s\n",gai_strerror(status));
    }    

    sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    bind(sockfd,res->ai_addr,res->ai_addrlen);
    listen(sockfd,10);

    addr_size = sizeof(their_addr);

    newfd = accept(sockfd,(struct sockaddr *)&their_addr,&addr_size);
    msg = "I am learning";
    len = sizeof(msg);
    byte_sent = send(newfd,msg,len,0);
    byte_received = recv(newfd,buffer,BUFFER_SIZE,0);

    shutdown(sockfd,2);
    shutdown(newfd,2);
    freeaddrinfo(res);
}