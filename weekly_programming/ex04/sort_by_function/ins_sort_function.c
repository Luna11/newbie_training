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
#define N 10000
#define M 10000

/* INTERNAL FUNCTION */
/* generate a random array */
int generate(int *a)
{
	int i, j, k, l;
//	int a[M]={0};
	int m=M;
	int flag=0;
	double start, finish, spend;
	FILE *fp=fopen("randam.txt", "w+");

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

	finish=(double)clock();
	spend=(finish-start)/CLOCKS_PER_SEC;

	printf("\nRandom data generate finished, please see randam.txt for detail.\n");
	printf("Generate time is: %.4fs\n", spend);

	for(l=0; l<M; l++)
	{
//		printf("%d\t", a[l]);
		fprintf(fp, "%d\t", a[l]);
	}
	
	fprintf(fp, "\ngenerate time is: %.4fs\n", spend);

	fclose(fp);
	fp=NULL;
	return 0;
}

int sort1(int *a)
{
	int i, j, temp, l;
	double start, finish, spend;
	FILE *fp = fopen("sort1.txt", "w+");
	
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

	finish = (double)clock();
	spend = (finish-start)/CLOCKS_PER_SEC;

	printf("\nSort1 finished, please see sort1.txt for detail.\n");
	printf("Sort1 time spend: %.4fs\n", spend);

	for(l=0; l<M; l++)
	{
//		printf("%d\t", b[l]);
		fprintf(fp, "%d\t", b[l]);
	}

	fprintf(fp, "\nsort1 time spend: %.4fs\n", spend);
	
	fclose(fp);
	fp = NULL;
	return 0;
}

int sort2(int *a)
{
	int i, j, temp, l;
	double start, finish, spend;
	FILE *fp = fopen("sort2.txt", "w+");

	start = (double)clock();
	
	int b[M] = {0};
	b[0] = a[0];

	for (i=1; i<M; i++) {
		temp = a[i];

		for (j=i; (j>0) && (temp<b[j-1]); j--) {
			b[j] = b[j-1];
		}

		b[j] = temp;
	}

	finish = (double)clock();
	spend = (finish-start)/CLOCKS_PER_SEC;

	printf("\nSort2 finished, please see sort2.txt for detail.\n");
	printf("Sort2 time spend: %.4fs\n", spend);

	for(l=0; l<M; l++)
	{
//		printf("%d\t", b[l]);
		fprintf(fp, "%d\t", b[l]);
	}

	fprintf(fp, "\nsort2 time spend: %.4fs\n", spend);
	
	fclose(fp);
	fp = NULL;
	return 0;
}

int main(void)
{
	int a[M] = {0};
	generate(a);
	sort1(a);
	sort2(a);
	return 0;
}
