/* Copyright (C) 2010 Shimomura Kimihiko <kshimo69@gmail.com> */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void p_err(char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  pid_t pid;
  char *arg[2];
  arg[0] = "/bin/sleep";
  arg[1] = "10";

  pid = fork();
  if (pid == 0) {
    /* child */
    printf("child start\n");
    sleep(10);
    printf("exec!\n");
    execv("/bin/sleep", arg);
    printf("child end\n");
    _exit(0);
  } else if(pid > 0) {
    /* parent */
    printf("parent sleep\n");
    sleep(20);
  } else {
    /* error */
    p_err("fork");
  }
  return 0;
}
