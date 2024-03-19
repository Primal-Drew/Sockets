
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "socket_descriptors.h"
#include <arpa/inet.h>


int main() {
    // Declare an IPv4 and IPv6 sockaddr structure
    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;

    // Set the IP addresses
    inet_pton(AF_INET, "10.12.110.57", &sa.sin_addr);
    inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &sa6.sin6_addr);

    // Convert the IP addresses to string format
    char ip4[INET_ADDRSTRLEN];
    char ip6[INET6_ADDRSTRLEN];

    inet_ntop(AF_INET, &sa.sin_addr, ip4, INET_ADDRSTRLEN);
    inet_ntop(AF_INET6, &sa6.sin6_addr, ip6, INET6_ADDRSTRLEN);

    // Print the IP addresses
    printf("IPv4 address: %s\n", ip4);
    printf("IPv6 address: %s\n", ip6);

    return 0;
}
