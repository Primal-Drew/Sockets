#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT "3490"
#define BACKLOG 10

void sigchld_handler(int s)
{
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
        errno = saved_errno;
    }
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)(sa))->sin_addr);

    return &(((struct sockaddr_in6*)(sa))->sin6_addr);
}

int main(void)
{
    int sockfd, newfd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];

    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("Server: socket");
            continue;
        }
        if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
        {
            perror("setsockopt:");
            exit(1);
        }
        if(bind(sockfd,p->ai_addr,p->ai_addrlen)==-1)
        {
            close(sockfd);
            perror("Server: Bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo);

    if (p=NULL)
    {
        fprintf(stderr,"Server failed to bind\n");
        exit(1);
    }

    if ( listen(sockfd,BACKLOG)== -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD,&sa,NULL)==-1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("Server waiting for connections...");

    while(1)
    {
        sin_size = sizeof(their_addr);
        newfd = accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);
        if(newfd == -1)
        {
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr*)&their_addr),s,sizeof(s));

        printf("Sever: Got connection from %s\n",s);


        if(!fork())
        {
            close(sockfd);
            if(send(newfd,"HELLO WORLD",12,0)==-1)
                perror("send");
            
            close(newfd);
            exit(0);
        }
        close(newfd);
    }
    return 0;
}