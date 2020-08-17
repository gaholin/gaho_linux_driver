obj-m := gahodrv.o


ifeq ($(KERNELDIR),)
KERNELDIR=/lib/modules/$(shell uname -r)/build
endif

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

insmod:
	sudo insmod ./gahodrv.ko

rmmod:
	sudo rmmod gahodrv

mknod:
	sudo mknod /dev/gahodrv c 60 0
	sudo chmod 666 /dev/gahodrv


app:
	gcc -o app app.c

clean_app:
	rm ./app

run:
	./app
