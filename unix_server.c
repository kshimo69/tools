/* unix_server.c
 * Server for UNIX Domain
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
  int sockfd, new_sockfd;
  int len;
  struct sockaddr_un s_addr;
  struct sockaddr_un c_addr;
  char buf[1024];

  printf("usage: %s\n", argv[0]);

  /* make socket */
  if((sockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  printf("socket: Success\n");

  /* clear s_addr */
  bzero((char *)&s_addr, sizeof(s_addr));

  /* set socket name */
  s_addr.sun_family = AF_UNIX;
  strcpy(s_addr.sun_path, SOCK_NAME);

  /* bind address */
  unlink(SOCK_NAME);
  if(bind(sockfd, (struct sockaddr *)&s_addr,
          sizeof(s_addr.sun_family) + strlen(SOCK_NAME)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  printf("bind: Success\n");

  /* listen socket */
  if(listen(sockfd, 1) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  printf("listen: Success\n");

  for(;;) {
    len = sizeof(c_addr);
    /* accept client */
    if((new_sockfd = accept(sockfd, (struct sockaddr *)&c_addr, &len)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    printf("accept: Success\n");
    close(sockfd);

    /* data receive */
    while(read(new_sockfd, buf, 1024)) {
      printf("-> %s", buf);
      write(new_sockfd, buf, 1024);
      printf("<- %s", buf);
    }
    close(new_sockfd);
  }
}
