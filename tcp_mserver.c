/* tcp_mserver.c
 * Server for INET Domain(TCP) multi client
 * http://www.ueda.info.waseda.ac.jp/~toyama/network/program/multi/mserver.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define PORT 12345
#define SOCK_MAX 5
#define UNUSED (-1)

void p_err(char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  int sockfd[SOCK_MAX + 1];
  int max = 0;  /* max num of used socket */
  int n = 0;
  int len, msg_len;
  fd_set readfds;
  struct sockaddr_in s_addr;
  struct sockaddr_in c_addr;
  char buf[1024];
  int i, j;

  printf("usage: %s\n", argv[0]);

  /* make socket */
  if((sockfd[0] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    p_err("socket");
  printf("socket: Success\n");

  /* clear s_addr */
  bzero((char *)&s_addr, sizeof(s_addr));

  /* set socket */
  s_addr.sin_family = AF_INET;
  s_addr.sin_addr.s_addr = INADDR_ANY;
  s_addr.sin_port = htons(PORT);

  /* bind address */
  if((bind(sockfd[0], (struct sockaddr *)&s_addr, sizeof(s_addr))) < 0)
    p_err("bind");
  printf("bind: Success\n");

  /* listen socket */
  if(listen(sockfd[0], SOCK_MAX) < 0)
    p_err("listen");
  printf("listen: Success\n");
  max = 1;  /* sockfd[0] is ready */

  while(1) {
    FD_ZERO(&readfds);
    //    printf("max: %d\n", max);

    for(i = 0; i < max; i++) {
      if(sockfd[i] != UNUSED) {
        FD_SET(sockfd[i], &readfds);
      }
    }

    if((n = select(FD_SETSIZE, &readfds, NULL, NULL, NULL)) < 0)
      p_err("select");
    //    printf("select: Success. returns: %d\n", n);

    for(i = 1; i < max; i++) {
      if(sockfd[i] != UNUSED) {
        if(FD_ISSET(sockfd[i], &readfds)) {
          printf("sockfd[%d] ready for reading\n", i);
          if((msg_len = read(sockfd[i], buf, sizeof(buf))) == -1) {
            p_err("read");
          } else if (msg_len != 0) {
            printf("--receive from client[%d]-> %s", i, buf);
            for(j = 1; j < max; j++) {  /* send msg for all client */
              if(sockfd[j] != UNUSED) {
                write(sockfd[j], buf, strlen(buf));
                printf("<-send to client[%d]-- %s", i, buf);
              }
            }
          } else {
            printf("client[%d]: connection closed.\n", i);
            close(sockfd[i]);
            sockfd[i] = UNUSED;
          }
        }
      }
    }

    if(FD_ISSET(sockfd[0], &readfds) != 0) {
      printf("Accept New one.\n");
      len = sizeof(c_addr);
      sockfd[max] = accept(sockfd[0], (struct sockaddr *)&c_addr, &len);
      printf("%d = accept()\n", sockfd[max]);
      if(sockfd[max] < 0)
        p_err("accept");
      if(max < SOCK_MAX) {
        printf("client accepted(%d).\n", max);
        max++;
      } else {
        printf("refuse connection.\n");
        strcpy(buf, "Server is too busy.\n");
        write(sockfd[max], buf, strlen(buf));
        close(sockfd[max]);
      }
    }
  }

  return(0);
}
