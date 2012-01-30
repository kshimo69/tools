#include <stdio.h>

int main() {
  int fd;
  char buf[1024];

  printf("hoge\n");
  fd = open("/dev/console");
  read(fd, buf, 1000);
  printf("%s", buf);
  close(fd);
}
