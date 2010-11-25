CC = gcc
CFLAGS = -g -Wall

TARGETS = daytimetcpcli daytimetcpcliv6 daytimetcpsrv

all: $(TARGETS)

lib.o: lib.h
	$(CC) -c lib.c

daytimetcpcli: daytimetcpcli.c lib.o
	$(CC) -o $@ $^

daytimetcpcliv6: daytimetcpcliv6.c lib.o
	$(CC) -o $@ $^

daytimetcpsrv: daytimetcpsrv.c lib.o
	$(CC) -o $@ $^

clean:
	rm -f $(TARGETS) *.o
