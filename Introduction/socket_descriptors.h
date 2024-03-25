#ifndef ADDRINFO_H
#define ADDRINFO_H

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

/**
 * @brief The addrinfo structure contains information about a socket address.
 *
 */
typedef struct addrinfo
{
    int ai_flags;             /**< AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST, AI_V4MAPPED, AI_ALL, AI_ADDRCONFIG */
    int ai_family;            /**< AF_UNSPEC, AF_INET, or AF_INET6 */
    int ai_socktype;          /**< SOCK_STREAM, SOCK_DGRAM, or SOCK_RAW */
    int ai_protocol;          /**< 0 or a protocol number */
    size_t ai_addrlen;        /**< Length of the ai_addr structure in bytes */
    struct sockaddr *ai_addr; /**< Pointer to a socket address structure */
    char *ai_canonname;       /**< Pointer to a null-terminated string containing the canonical name for the host */
    struct addrinfo *ai_next; /**< Pointer to the next structure in a linked list */

} addrinfo;

/**
 * @brief The sockaddr structure contains the address of a socket.
 */
typedef struct sockaddr
{
    unsigned short sa_family; /**< Address family, such as AF_INET or AF_INET6 */
    char sa_data[14];         /**< 14 bytes of protocol-specific address information */

} sockaddr;

/**
 * @brief The sockaddr_in structure defines the address format for IPv4 addresses.
 *
 */
typedef struct sockaddr_in
{
    unsigned short sin_family; /**< Address family, always AF_INET */
    unsigned short sin_port;   /**< Port number in network byte order */
    struct in_addr sin_addr;   /**< IPv4 address */
    char sin_zero[8];          /**< Reserved for future use; must be set to zero */

} sockaddr_in;

/**
 * @brief The in_addr structure defines the address format for IPv4 addresses.
 *
 */
typedef struct in_addr
{
    uint32_t s_addr; /**< IPv4 address in network byte order */

} in_addr;

/**
 * @brief The sockaddr_in6 structure defines the address format for IPv6 addresses.
 */
typedef struct sockaddr_in6
{
    unsigned short sin6_family; /**< Address family, always AF_INET6 */
    unsigned short sin6_port;   /**< Port number in network byte order */
    uint32_t sin6_flowinfo;     /**< IPv6 flow information */
    struct in6_addr sin6_addr;  /**< IPv6 address */
    uint32_t sin6_scope_id;     /**< IPv6 scope ID */

} sockaddr_in6;

/**
 * @brief The in6_addr structure defines the address format for IPv6 addresses.
 */
typedef struct in6_addr
{
    uint8_t s6_addr[16]; /**< IPv6 address in network byte order */
} in6_addr;

/**
 * @brief The sockaddr_storage structure contains the address and address family of a socket.
 */
typedef struct sockaddr_storage
{
    sa_family_t ss_family;       /**< Address family */
    char __ss_pad1[_SS_PADSIZE]; /**< Padding for future expansion */
    int64_t __ss_align;          /**< Force the structure to be 64-bit aligned */
    char __ss_pad2[_SS_PADSIZE]; /**< Padding for future expansion. */
} sockaddr_storage;


#endif
