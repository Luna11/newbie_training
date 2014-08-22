#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/ioctl.h>
#include <fcntl.h>
#include "misc_cmd.h"

int main()
{
	printf("now, let's open the device miscdev, starting......\n");

	int fd=open("/dev/miscdev", O_RDWR);
	
	if(fd==-1){
		perror("open error");
		return -1;
	}
	else{
		printf("device miscdev open\n");
	}

	close(fd);

	return 0;
}
