/* udp_client.c
 * Client for INET Domain(UDP)
 * http://www.ueda.info.waseda.ac.jp/~toyama/network/protocol.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 12345

void p_err(char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  struct sockaddr_in addr;
  struct hostent *host;
  int sockfd;
  char buf[1024];
  int buflen;
  int ret;

  if(argc != 2) {
    printf("usage: %s SERVER_NAME\n", argv[0]);
    exit(1);
  }

  /* make socket */
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    p_err("socket");
  printf("socket: Success\n");

  /* clear s_addr */
  bzero((char *)&addr, sizeof(addr));

  /* set socket */
  if((host = gethostbyname(argv[1])) == NULL)
    p_err("gethostbyname");
  bcopy(host->h_addr, &addr.sin_addr, host->h_length);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);

  /* data send */
  while(fgets(buf, 1024, stdin)) {
    buflen = strlen(buf);
    if(sendto(sockfd, buf, buflen, 0, (struct sockaddr *)&addr, sizeof(addr)) != buflen)
      p_err("sendto");
    if((ret = recvfrom(sockfd, buf, 1024, 0, NULL, &buflen)) < 0)
      p_err("recvfrom");
    buf[ret] = '\0';
    printf("-> %s", buf);
  }
  close(sockfd);
  exit(EXIT_SUCCESS);

  return(0);
}
