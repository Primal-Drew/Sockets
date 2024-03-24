#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "8888"
#define MAX 900
#define HOST "192.168.100.20"

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)(sa))->sin_addr);
    }
    return &(((struct sockaddr_in6 *)(sa))->sin6_addr);
}

void sigchld_handler(int s)
{
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
        errno = saved_errno;
    }
}

int main()
{
    int yes = 1;
    int numbytes;
    int sockfd, newfd;
    int status;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    struct sockaddr *theirs_addr = (struct sockaddr *)&their_addr;
    struct sigaction sa;
    socklen_t sin_size;
    char command_buf[MAX];
    char buf[MAX];
    char target_ip[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;

    if (status=getaddrinfo(HOST, PORT, &hints, &servinfo) == -1)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
    {
        perror("Server: socket");
        return 1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("Server: setsockopt");
        close(sockfd);
        return 1;
    }

    if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        perror("Server: bind");
        close(sockfd);
        return 1;
    }

    freeaddrinfo(servinfo);

    if (listen(sockfd, 5) == -1)
    {
        perror("Server: listen");
        close(sockfd);
        return 1;
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("Server: sigaction");
        close(sockfd);
        return 1;
    }

    printf("Server now waiting for connections...\n");

    while (1)
    {
        sin_size = sizeof(their_addr);

        newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1)
        {
            perror("Server: accept");
            close(sockfd);
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr(theirs_addr), target_ip, sizeof(target_ip));
        printf("Connection from: %s\n", target_ip);

        if (!fork())
        {
            close(sockfd);


            printf("Enter command: ");
            if (fgets(command_buf, sizeof(command_buf), stdin) == NULL)
            {
                fprintf(stderr, "Error reading command\n");
                exit(EXIT_FAILURE);
            }
            command_buf[strcspn(command_buf, "\n")] = '\0';

            // Send command to client
            if (send(newfd, command_buf, strlen(command_buf), 0) == -1)
            {
                perror("Server: send command");
                close(newfd);
                exit(EXIT_FAILURE);
            }

            // Receive feedback acknowledgment from client
            if ((numbytes = recv(newfd, buf, sizeof(buf) - 1, 0)) == -1)
            {
                perror("Server: recv feedback acknowledgment");
                close(newfd);
                exit(EXIT_FAILURE);
            }
            buf[numbytes] = '\0';
            printf("Feedback acknowledgment from client: %s\n", buf);

            // Receive result of the command executed by client
            if ((numbytes = recv(newfd, buf, sizeof(buf) - 1, 0)) == -1)
            {
                perror("Server: recv command result");
                close(newfd);
                exit(EXIT_FAILURE);
            }
            buf[numbytes] = '\0';
            printf("Result of command executed by client: %s\n", buf);

            close(newfd);
            exit(EXIT_SUCCESS);
        }
        close(newfd);
    }

    return 0;
}
