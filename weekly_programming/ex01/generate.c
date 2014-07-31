/*------------------------------------------
	File name:
		generate.c

	Description:
		generate M non-repeated randoms between 1 to N

	Author/Created Date:
		Luna Wang, 2014/7/24

	Modification history:

	note:
		M<=N
		

-------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <fcntl.h>

#define N 10
#define M 10

int main(void)
{
	int i, j, k, l;
	char a[M]={0};
	int m=M;
	int flag=0;
//	int fp=open("gfile.txt", O_RDWR | O_CREAT | O_TRUNC, 0664);
	FILE *fp=fopen("gfile.txt", "w+");


	srand((int)time(0));
	a[0]=1+rand()%N;

	while(m-1)
	{
		j=1+rand()%N;
		for(k=0; k<=(M-m); k++)
		{
			if(j==a[k])
			{
				flag=1;
				break;
			}
		}

		if(!flag)
		{
			a[M-m+1]=j;
			m--;
		}
		else
		{
			flag=0;
		}
	}

	for(l=0; l<M; l++)
	{
		printf("%d\t", a[l]);
		fprintf(fp, "%d\n", a[l]);
	}
	

	fclose(fp);
	fp=NULL;
	return 0;
}
