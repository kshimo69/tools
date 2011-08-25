/* Copyright (C) 2011 Shimomura Kimihiko <kshimo69@gmail.com> */
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	int i;
	int data[] = {1,2,3,4,5,6,7,8,9,10};
	FILE *fp;

	if ((fp = fopen("hoge_fwrite.txt", "w")) == NULL) {
		perror("fopen");
		exit(1);
	}
	for (i = 0; i < 110; i++) {
		if ((fwrite(data, sizeof(int), 10, fp)) < 10) {
			if (feof(fp) != 0) {
				printf("end-of-file\n");
				exit(1);
			}
			if (ferror(fp) != 0) {
				printf("error detected.\n");
				perror("fwrite");
				exit(1);
			}
		}
		printf("fwrite %d bytes done.\n", sizeof(int) * 10 * (i + 1));
	}
	if (fclose(fp) != 0) {
		perror("fclose");
		exit(1);
	}

	if ((fp = fopen("hoge_fprintf.txt", "w")) == NULL) {
		perror("fopen");
		exit(1);
	}
	for (i = 0; i < 410; i++) {
		if ((fprintf(fp, "%s\n", "hoge-hoge")) < 0) {
			printf("error\n");
			exit(1);
		}
		printf("fprintf %d bytes done.\n", sizeof(char) * 10 * (i + 1));
	}
	if (fclose(fp) != 0) {
		perror("fclose");
		exit(1);
	}

	if ((fp = fopen("hoge_fputs.txt", "w")) == NULL) {
		perror("fopen");
		exit(1);
	}
	for (i = 0; i < 410; i++) {
		if ((fputs("hoge-hoge\n", fp)) == EOF) {
			printf("end-of-file\n");
			exit(1);
		}
		printf("fputs %d bytes done.\n", sizeof(char) * 10 * (i + 1));
	}
	if (fclose(fp) != 0) {
		perror("fclose");
		exit(1);
	}

	for (i = 0; i < 410; i++) {
		if ((fprintf(stdout, "%s\n", "hoge-hoge")) < 0) {
			printf("error\n");
			exit(1);
		}
		printf("fprintf %d bytes done.\n", sizeof(char) * 10 * (i + 1));
	}
}
