#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd;
	int counter = 0;
	int old_counter = 0;

	fd = open("/dev/second0", O_RDONLY);
	if(fd != -1) {
		while(1) {
			read(fd, &counter, sizeof(int));
			if(counter != old_counter) {
				printf("seconds after open /dev/second0: %d\n", counter);
				old_counter = counter;
			}
		}
	}
	else {
	printf("open fail!!\n");
	}
}
