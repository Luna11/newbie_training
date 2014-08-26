/*------------------------------------------
	File name:
		ins_sort.c

	Description:
		A file to sort numbers with insert function.

	Author/Created Date:
		Luna Wang, Aug26'14

	Modification history:

	note:
			

-------------------------------------------*/
/* INCLUDE FILES */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* DEFINE */
#define N 10
#define M 9

/* INTERNAL FUNCTION */
/* generate a random array */
int generate(int *a)
{
	int i, j, k, l;
//	int a[M]={0};
	int m=M;
	int flag=0;
	double start, finish, spend;
//	FILE *fp=fopen("gfile.txt", "w+");

	start=(double)clock();

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

	printf("Generate random data: \n");

	for(l=0; l<M; l++)
	{
		printf("%d\t", a[l]);
//		fprintf(fp, "%d\n", a[l]);
	}
	
	finish=(double)clock();

	spend=(finish-start)/CLOCKS_PER_SEC;
	printf("\ntime spend: %.4fs\n", spend);

//	fclose(fp);
//	fp=NULL;
	return 0;
}

int sort(int *a)
{
	int i, j, temp, l;
	double start, finish, spend;

	start = (double)clock();
	
	int b[M] = {0};
	b[0] = a[0];

	for (i=1; i<M; i++) {
		j = i;
		b[j] = a[i];

		for ( ; (j>0) && (b[j]<b[j-1]); j--) {
			temp = b[j-1];
			b[j-1] = b[j];
			b[j] = temp;
		}
	}

	printf("After sort, the numbers are:\n");

	for(l=0; l<M; l++)
	{
		printf("%d\t", b[l]);
//		fprintf(fp, "%d\n", b[l]);
	}

	finish = (double)clock();

	spend = (finish-start)/CLOCKS_PER_SEC;
	printf("\ntime spend: %.4fs\n", spend);
	
	return 0;
}

int main(void)
{
	int a[M] = {0};
	generate(a);
	sort(a);
	return 0;
}
