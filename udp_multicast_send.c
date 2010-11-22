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
    /*in_addr_t ipaddr;*/
    struct in_addr ipaddr;

    char buf[1024];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
        perror("socket");
        exit(1);
    }

    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = inet_addr("239.192.1.2");

    /*ipaddr = inet_addr("127.0.0.1");*/
    ipaddr.s_addr = inet_addr("127.0.0.1");
    if(setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF,
                (char *)&ipaddr, sizeof(ipaddr)) < 0) {
        perror("setsockopt");
        close(sock);
        exit(1);
    }
    /*
    if(sendto(sock, "HELLO", 6, 0,
                (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("sendto");
        close(sock);
        exit(1);
    }
    */
    while(fgets(buf, sizeof(buf), stdin)) {
        sendto(sock, buf, sizeof(buf), 0,
                (struct sockaddr *)&addr, sizeof(addr));
        printf("-- send -> %s", buf);
    }
    close(sock);

    exit(0);
}
