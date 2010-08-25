obj-m	:= msg_module.o

KVER	:= $(shell uname -r)
ARCH	:= $(shell uname -m)
ROOTDIR	:= /usr/src/kernels/$(KVER)-$(ARCH)/
PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(ROOTDIR) M=$(PWD) modules

clean:
	rm -f *.o *.ko
