#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>

void fincore(char *filename) {
    int fd;
    struct stat st;
    void *pa = (char *)0;
    char *vec = (char *)0;
    register size_t n = 0;
    size_t pageSize = getpagesize();
    register size_t pageIndex;
    int sum = 0;

    printf("pagesize: %ld\n", pageSize);

    fd = open(filename, 0);
    if(fd < 0) {
        perror("open");
        return;
    }

    if(fstat(fd, &st) != 0) {
        perror("fstat");
        close(fd);
        return;
    }

    printf("filesize: %ld\n", st.st_size);

    pa = mmap((void *)0, st.st_size, PROT_NONE, MAP_SHARED, fd, 0);
    if(pa == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }

    printf("page num: %ld\n", (st.st_size + pageSize - 1) / pageSize);

    vec = calloc(1, (st.st_size + pageSize - 1) / pageSize);
    if(vec == (void *)0) {
        perror("calloc");
        close(fd);
        return;
    }

    if(mincore(pa, st.st_size, vec) != 0) {
        fprintf(stderr, "mincore(%p, %lu, %p): %s\n",
                pa, (unsigned long)st.st_size, vec, strerror(errno));
        free(vec);
        close(fd);
        return;
    }

    for(pageIndex=0; pageIndex<=st.st_size/pageSize; pageIndex++) {
        if(vec[pageIndex]&1) {
            sum++;
        }
    }

    free(vec);
    vec = (char *)0;

    munmap(pa, st.st_size);
    close(fd);

    printf("in memory pages: %d\n", sum);
    return;
}

int main(int argc, char **argv)
{
    fincore(argv[1]);
}
