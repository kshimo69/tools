/* udp_server.c
 * Server for INET Domain(UDP)
 * http://www.ueda.info.waseda.ac.jp/~toyama/network/program/dgram/dserver.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345

void p_err(char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  int sockfd;
  int len;
  struct sockaddr_in s_addr;
  struct sockaddr_in c_addr;
  char buf[1024];
  int ret;

  printf("usage: %s\n", argv[0]);

  /* make socket */
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    p_err("socket");
  printf("socket: Success\n");

  /* clear s_addr */
  bzero((char *)&s_addr, sizeof(s_addr));

  /* set socket */
  s_addr.sin_family = AF_INET;
  s_addr.sin_addr.s_addr = INADDR_ANY;
  s_addr.sin_port = htons(PORT);

  /* bind address */
  if(bind(sockfd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0)
    p_err("bind");
  printf("bind: Success\n");

  len = sizeof(c_addr);

  /* data receive */
  while(1) {
    if((ret = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *)&c_addr, &len)) < 0)
      p_err("recvfrom");
    printf("--receive-> %s", buf);
    if(sendto(sockfd, buf, ret, 0, (struct sockaddr *)&c_addr, len) != ret)
      p_err("sendto");
    printf("<-send to-- %s", buf);
  }

  return(0);
}
