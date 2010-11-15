/* unix_client.c
 * Client for UNIX Domain
 * http://www.ueda.info.waseda.ac.jp/~toyama/network/example1.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_NAME "/tmp/socket"

int main(int argc, char *argv[])
{
  struct sockaddr_un addr;
  int sockfd;
  char buf[1024];
  int ret = 1024;

  printf("usage: %s\n", argv[0]);

  /* make socket */
  if((sockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  printf("socket: Success\n");

  /* clear s_addr */
  bzero((char *)&addr, sizeof(addr));

  /* set socket name */
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, SOCK_NAME);

  /* connect server */
  if(connect(sockfd, (struct sockaddr *)&addr,
             sizeof(addr.sun_family) + strlen(SOCK_NAME)) < 0) {
    perror("connect");
    exit(EXIT_FAILURE);
  }
  printf("connect: Success\n");

  /* data send */
  while(fgets(buf, 1024, stdin)) {
    write(sockfd, buf, 1024);
    ret = read(sockfd, buf, ret);
    printf("-> %s", buf);
  }
  close(sockfd);
  exit(EXIT_SUCCESS);
}
