#include "lib.h"

int Socket(int family, int type, int protocol)
{
    int n;
    if((n = socket(family, type, protocol)) < 0) {
        perror("socket");
        exit(1);
    }
    return(n);
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if(bind(fd, sa, salen) < 0) {
        perror("bind");
        exit(1);
    }
}

void Listen(int fd, int backlog)
{
    char *ptr;
    if((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);

    if(listen(fd, backlog) < 0) {
        perror("listen");
        exit(1);
    }
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int     n;

again:
    if((n = accept(fd, sa, salenptr)) < 0) {
        if(errno == EPROTO || errno == ECONNABORTED)
            goto again;
        else {
            perror("accept");
            exit(1);
        }
    }
    return(n);
}

void Write(int fd, void *ptr, size_t nbytes)
{
    if(write(fd, ptr, nbytes) != nbytes) {
        perror("write");
        exit(1);
    }
}

void Close(int fd)
{
    if(close(fd) == -1) {
        perror("close");
        exit(1);
    }
}

int sock_bind_wild(int sockfd, int family)
{
    socklen_t   len;

    switch(family) {
    case AF_INET: {
        struct sockaddr_in  sin;

        bzero(&sin, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        sin.sin_port = htons(0);    /* bind ephemeral port */

        if(bind(sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
            return(-1);
        len = sizeof(sin);
        if(getsockname(sockfd, (struct sockaddr *)&sin, &len) < 0)
            return(-1);
        return(sin.sin_port);
        }

    case AF_INET6: {
        struct sockaddr_in6 sin6;

        bzero(&sin6, sizeof(sin6));
        sin6.sin6_family = AF_INET6;
        sin6.sin6_addr = in6addr_any;
        sin6.sin6_port = htons(0);  /* bind ephemeral port */

        if(bind(sockfd, (struct sockaddr *)&sin6, sizeof(sin6)) < 0)
            return(-1);
        len = sizeof(sin6);
        if(getsockname(sockfd, (struct sockaddr *) &sin6, &len) < 0)
            return(-1);
        return(sin6.sin6_port);
        }
    }
    return(-1);
}

int sock_cmp_addr(const struct sockaddr *sa1, const struct sockaddr *sa2,
             socklen_t salen)
{
    if(sa1->sa_family != sa2->sa_family)
        return(-1);

    switch(sa1->sa_family) {
    case AF_INET: {
        return(memcmp(&((struct sockaddr_in *) sa1)->sin_addr,
                      &((struct sockaddr_in *) sa2)->sin_addr,
                      sizeof(struct in_addr)));
        }

    case AF_INET6: {
        return(memcmp(&((struct sockaddr_in6 *) sa1)->sin6_addr,
                      &((struct sockaddr_in6 *) sa2)->sin6_addr,
                      sizeof(struct in6_addr)));
        }

    case AF_UNIX: {
        return(strcmp(((struct sockaddr_un *) sa1)->sun_path,
                      ((struct sockaddr_un *) sa2)->sun_path));
        }
    }
    return(-1);
}

int sock_cmp_port(const struct sockaddr *sa1, const struct sockaddr *sa2,
             socklen_t salen)
{
    if(sa1->sa_family != sa2->sa_family)
        return(-1);

    switch(sa1->sa_family) {
    case AF_INET: {
        return(((struct sockaddr_in *) sa1)->sin_port ==
               ((struct sockaddr_in *) sa2)->sin_port);
        }

    case AF_INET6: {
        return(((struct sockaddr_in6 *) sa1)->sin6_port ==
               ((struct sockaddr_in6 *) sa2)->sin6_port);
        }
    }
    return(-1);
}

int sock_get_port(const struct sockaddr *sa, socklen_t salen)
{
    switch(sa->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in *) sa;
        return(sin->sin_port);
        }

    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) sa;
        return(sin6->sin6_port);
        }
    }
    return(-1);
}

char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char portstr[7];
    static char str[128];

    switch(sa->sa_family) {
        case AF_INET: {
            struct sockaddr_in *sin = (struct sockaddr_in *) sa;
            if(inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
                return(NULL);
            if(ntohs(sin->sin_port) != 0) {
                snprintf(portstr, sizeof(portstr), ".%d", ntohs(sin->sin_port));
                strcat(str, portstr);
            }
            return(str);
            }
        case AF_INET6: {
            struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) sa;
            if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL)
                return(NULL);
            if (ntohs(sin6->sin6_port) != 0) {
                snprintf(portstr, sizeof(portstr), ".%d", ntohs(sin6->sin6_port));
                strcat(str, portstr);
            }
            return(str);
            }
        case AF_UNIX: {
            struct sockaddr_un *unp = (struct sockaddr_un *) sa;
            /* OK to have no pathname bound to the socket: happens on
               every connect() unless client calls bind() first. */
            if (unp->sun_path[0] == 0)
                strcpy(str, "(no pathname bound)");
            else
                snprintf(str, sizeof(str), "%s", unp->sun_path);
            return(str);
            }
        default:
            snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d", sa->sa_family, salen);
            return(str);
    }
    return(NULL);
}

void sock_set_addr(const struct sockaddr *sa, socklen_t salen, const void *addr)
{
    switch(sa->sa_family) {
    case AF_INET: {
        struct sockaddr_in  *sin = (struct sockaddr_in *) sa;
        memcpy(&sin->sin_addr, addr, sizeof(struct in_addr));
        return;
        }

    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) sa;
        memcpy(&sin6->sin6_addr, addr, sizeof(struct in6_addr));
        return;
        }
    }
    return;
}

void sock_set_port(const struct sockaddr *sa, socklen_t salen, int port)
{
    switch(sa->sa_family) {
    case AF_INET: {
        struct sockaddr_in      *sin = (struct sockaddr_in *) sa;
        sin->sin_port = port;
        return;
        }

    case AF_INET6: {
        struct sockaddr_in6     *sin6 = (struct sockaddr_in6 *) sa;
        sin6->sin6_port = port;
        return;
        }
    }
    return;
}

void sock_set_wild(const struct sockaddr *sa, socklen_t salen)
{
    const void  *wildptr;

    switch(sa->sa_family) {
    case AF_INET: {
        static struct in_addr   in4addr_any;
        in4addr_any.s_addr = htonl(INADDR_ANY);
        wildptr = &in4addr_any;
        break;
        }

    case AF_INET6: {
        wildptr = &in6addr_any;
        break;
        }
    }
    sock_set_addr(sa, salen, wildptr);
    return;
}
