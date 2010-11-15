#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <linux/fs.h>

struct flock64 {
        short  l_type;
        short  l_whence;
        loff_t l_start;
        loff_t l_len;
        pid_t  l_pid;
};


int main(int argc, char **argv)
{
    struct flock64 fl;
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

    if (fcntl(fd, F_SETLKW64, &fl) == -1) {
        perror("fcntl");
        exit(1);
    }

    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK64, &fl) == -1) {
        perror("fcntl");
        exit(1);
    }

    close(fd);

    return 0;
}

