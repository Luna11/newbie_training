/*------------------------------------------
	File name:
		generate.c

	Description:
		generate random data

	Author/Created Date:
		Luna Wang, 2014/7/24

	Modification history:
		no

-------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <fcntl.h>

#define N 100

int main(void)
{
	int i, j;
	char a[N]={0};
//	int fp=open("gfile.txt", O_RDWR | O_CREAT | O_TRUNC, 0664);
    FILE *fp=fopen("gfile.txt", "w+");


	srand((int)time(0));
	for(i=0; i<N; i++)
	{
		
		j=1+rand()%N;
		if(a[j-1]==0)
		{
			a[j-1]=1;
		}
		else
		{
			i--;
			continue;
		}
//		printf("%d \t", j);
		fprintf(fp, "%d\n", j);
	}

	fclose(fp);
	fp=NULL;
	return 0;
}
