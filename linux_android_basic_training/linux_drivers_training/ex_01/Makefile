#Filename:
#	Makefile
#
#Description:
#	This is the Makefile of the hello.c.
#
#Author/Create Date:
#	Luna Wang, Jul 09,2014
#
#Modification History:
#	None

ifneq ($(KERNELRELEASE),)
	obj-m:=hello.o

else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) 

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
endif
