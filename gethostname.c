#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/utsname.h>

int main(int argc, char *argv)
{
  struct utsname *utsname;
  char hostname[64];
  struct hostent *host;
  int i;
  struct in_addr *ip;

  utsname = (struct utdname *)malloc(sizeof(struct utsname));
  if(uname(utsname) != 0) {
    perror("uneme");
  } else {
    printf("uname-> %s\n", utsname->nodename);
  }

  if(gethostname(hostname, sizeof(hostname)) != 0) {
    perror("gethostname");
  } else {
    printf("gethostname-> %s\n", hostname);
  }

  host = gethostbyname(hostname);
  if(host == NULL) {
    perror("gethostbyname");
  } else {
    printf("gethostbyname-> %s\n", host->h_name);
    for(i = 0; host->h_aliases[i]; i++) {
      printf("alias_name-> %s\n", host->h_aliases[i]);
    }
    for(i = 0; host->h_addr_list[i]; i++) {
      ip = (struct in_addr *)host->h_addr_list[i];
      printf("ip_addr-> %s\n", inet_ntoa(*ip));
    }
  }
}
