/* tcp_client.c
 * Client for INET Domain(TCP)
 * http://www.ueda.info.waseda.ac.jp/~toyama/network/example2.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
  int ret;
  unsigned int optval = 1;

  if(argc != 2) {
    printf("usage: %s SERVER_NAME\n", argv[0]);
    exit(1);
  }

  /* make socket */
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    p_err("socket");
  printf("socket: Success\n");

  /* setsockopt */
  if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0)
    p_err("setsockopt");
  printf("setsockopt: Success\n");

  /* clear s_addr */
  bzero((char *)&addr, sizeof(addr));

  /* set socket */
  if((host = gethostbyname(argv[1])) == NULL)
    p_err("gethostbyname");
  bcopy(host->h_addr, &addr.sin_addr, host->h_length);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);

  /* connect server */
  if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    p_err("connect");
  printf("connect: Success\n");

  /* data send */
  while(fgets(buf, 1024, stdin)) {
    write(sockfd, buf, 1024);
    ret = read(sockfd, buf, ret);
    printf("-> %s", buf);
  }
  close(sockfd);
  exit(EXIT_SUCCESS);

  return(0);
}
