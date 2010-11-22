#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char const* argv[])
{
    int sock;
    struct sockaddr_in addr;
    struct ip_mreq mreq;

    char buf[1024];
    int reuse = 1;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
        perror("socket");
        exit(1);
    }

    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                (char *)&reuse, sizeof(reuse)) < 0) {
        perror("setsockopt REUSE");
        close(sock);
        exit(1);
    }

    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    /*addr.sin_addr.s_addr = inet_addr("127.0.0.1");*/

    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        exit(1);
    }

    /* must do setsockopt after bind */
    /*memset(&mreq, 0, sizeof(mreq));*/
    /*mreq.imr_interface.s_addr = INADDR_ANY;*/
    mreq.imr_interface.s_addr = inet_addr("127.0.0.1");
    mreq.imr_multiaddr.s_addr = inet_addr("239.192.1.2");

    if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                (char *)&mreq, sizeof(mreq)) < 0) {
        perror("setsockopt JOIN");
        close(sock);
        exit(1);
    }

    memset(buf, 0, sizeof(buf));
    /*
    recv(sock, buf, sizeof(buf), 0);
    printf("%s\n", buf);
    */
    while(recv(sock, buf, sizeof(buf), 0)) {
        printf("-- recv -> %s", buf);
    }
    close(sock);

    exit(0);
}
