#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/ioctl.h>
#include <fcntl.h>
#include "misc_cmd.h"

/*  remove to file misc_cmd.h  */
/*
#define MISC_IOC_MAGIC  'y'
#define MISC_IOC_MAXNR  3
#define MISC_READ  _IOR(MISC_IOC_MAGIC, 1, int)
#define MISC_WRITE  _IOW(MISC_IOC_MAGIC, 2, int)
#define MISC_IOC_TEST  _IO(MISC_IOC_MAGIC,3)
*/

int main()
{
	int rdata=0;
	int wdata=2011;
	char buf[20];
	int fd=open("/dev/miscdev", O_RDWR);
	if(fd==-1){
		perror("open");
		return -1;
	}
	
	/* WRITE & LSEEK & READ FUNCTION TEST  */
	if(write(fd, "1234567890-1-2-3-4-5-6-7-8", 50)!=-1){

		lseek(fd, 10, 0);   //f_pos point to the first '-'
		if(read(fd, buf, 10)!=-1) printf("%s\n", buf);  //-1-2-3-4-5
	
		lseek(fd, 0, 0);    //f_pos point to beginning '1'
		if(read(fd, buf, 5)!=-1) printf("%s\n",buf);   //123453-4-5

		lseek(fd, 0, 1);    //f_pos point to '6'
		if(read(fd, buf, 10)!=-1) printf("%s\n", buf);    //67890-1-2-

		lseek(fd, 5, 1);
		if(read(fd, buf, 10)!=-1) printf("%s\n", buf);    //-6-7-8
	}

	/* READ FROM KERNEL DRIVER TO &RDATA  */
	if(ioctl(fd, MISC_READ, &rdata) != -1){
		printf("read:%d\n", rdata);
		sleep(1);
	}
	
	/* WRITE TO KERNEL FROM WDATA  */
	if(ioctl(fd, MISC_WRITE, &wdata) != -1){
		 printf("write:%d\n", wdata);
		 sleep(1);
	}
	
	/* test  */
	if(!ioctl(fd, MISC_IOC_TEST)){
		printf("misc-ioc-test is ok\n");
		sleep(1);
	}

	return 0;
}
