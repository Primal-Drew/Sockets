#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int status; // Stores the value returned by the gai_strerror.

    /**
     * @param hints - Points to a addrinfo that you have already filled with relevant info.
    */
    struct addrinfo hints;

    /**
     * @param servinfo - A linked list with all kind 
    */
    struct addrinfo *servinfo; // Will point to the results.

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;// AI_PASSIVE is the IP of your local host.

    // getaddrinfo returns a non-zero value when an error is encountered.
    if ((status=getaddrinfo(NULL,"3490", &hints,&servinfo))!=0)
    {
        fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
        exit(1);
    }

    // @servinfo is a linked list thus must be freed.
    freeaddrinfo(servinfo);
}