CC = gcc
CFLAGS = -g -Wall

TARGETS = daytimetcpcli daytimetcpcliv6 daytimetcpsrv daytimetcpsrv_print_client

all: $(TARGETS)

lib.o: lib.h lib.c
	$(CC) -c lib.c

daytimetcpcli: daytimetcpcli.c lib.o
	$(CC) -o $@ $^

daytimetcpcliv6: daytimetcpcliv6.c lib.o
	$(CC) -o $@ $^

daytimetcpsrv: daytimetcpsrv.c lib.o
	$(CC) -o $@ $^

daytimetcpsrv_print_client: daytimetcpsrv_print_client.c lib.o
	$(CC) -o $@ $^

clean:
	rm -f $(TARGETS) *.o
