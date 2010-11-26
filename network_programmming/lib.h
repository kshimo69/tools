#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <time.h>

#define MAXLINE 4096
#define LISTENQ 1024

int Socket(int family, int type, int protocol);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Write(int fd, void *ptr, size_t nbytes);
void Close(int fd);
int sock_bind_wild(int sockfd, int family);
int sock_cmp_addr(const struct sockaddr *sockaddr1,
        const struct sockaddr *sockaddr2, socklen_t addrlen);
int sock_cmp_port(const struct sockaddr *sockaddr1,
        const struct sockaddr *sockaddr2, socklen_t addrlen);
int sock_get_port(const struct sockaddr *sockaddr, socklen_t addrlen);
char *sock_ntop(const struct sockaddr *sockaddr, socklen_t addrlen);
void sock_set_addr(const struct sockaddr *sockaddr,
        socklen_t addrlen, const void *ptr);
void sock_set_port(const struct sockaddr *sockaddr,
        socklen_t addrlen, int port);
void sock_set_wild(const struct sockaddr *sockaddr, socklen_t addrlen);
const char * Inet_ntop(int family, const void *addrptr, char *strptr, size_t len);
void Inet_pton(int family, const char *strptr, void *addrptr);
