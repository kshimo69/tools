#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
 
main(int argc, char **argv)
{
    struct flock fl;
    int fd;
 
    fl.l_type   = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start  = 0;
    fl.l_len    = 0;
    fl.l_pid    = getpid();
 
    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror("open");
        exit(1);
    }
 
    printf("LOCK");
    getchar();
    fputs(".....", stdout);
 
    if (fcntl(fd, F_SETLKW, &fl) == -1) {
        perror("fcntl");
        exit(1);
    }
 
    puts("OK");
    printf("UNLOCK");
    getchar();
    fputs(".....", stdout);
 
    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        perror("fcntl");
        exit(1);
    }
 
    puts("OK");
 
    close(fd);
 
    return 0;
}
