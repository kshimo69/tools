obj-m	:= msg_module.o

KVER	:= $(shell uname -r)
ARCH	:= $(shell uname -m)
ROOTDIR	:= /usr/src/kernels/$(KVER)-$(ARCH)/  # RHEL6
#ROOTDIR	:= /usr/src/kernels/$(KVER)/      # RHEL5
PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(ROOTDIR) M=$(PWD) modules

clean:
	rm -f *.o *.ko
