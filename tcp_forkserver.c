/* tcp_forkserver.c
 * Server for INET Domain(TCP) multi client (use fork)
 * http://www.ueda.info.waseda.ac.jp/~toyama/network/fork.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
  int sockfd, new_sockfd;
  int len;
  struct sockaddr_in s_addr;
  struct sockaddr_in c_addr;
  char buf[1024];
  int pid;

  printf("usage: %s\n", argv[0]);

  /* make socket */
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    p_err("socket");
  printf("socket: Success\n");

  /* clear s_addr */
  bzero((char *)&s_addr, sizeof(s_addr));

  /* set socket */
  s_addr.sin_family = AF_INET;
  s_addr.sin_addr.s_addr = INADDR_ANY;
  s_addr.sin_port = htons(PORT);

  /* bind address */
  if((bind(sockfd, (struct sockaddr *)&s_addr, sizeof(s_addr))) < 0)
    p_err("bind");
  printf("bind: Success\n");

  /* listen socket */
  if(listen(sockfd, 1) < 0)
    p_err("listen");
  printf("listen: Success\n");

  len = sizeof(c_addr);

  for(;;) {
    if((new_sockfd = accept(sockfd, (struct sockaddr *)&c_addr, &len)) < 0)
      p_err("accept");
    printf("accept: Success\n");

    if((pid = fork()) < 0) {
      p_err("fork");
    } else if (pid == 0) {
      printf("fork: Success\n");
      close(sockfd);
      while(read(new_sockfd, buf, 1024)) {
        printf("--receive-> %s", buf);
        write(new_sockfd, buf, 1024);
        printf("<-send to-- %s", buf);
      }
      exit(0);
    }
    close(new_sockfd);
  }

  return(0);
}
