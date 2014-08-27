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

/* global variable */
double spend = 0;
int g[M] = {0};

/* INTERNAL FUNCTION */
/* generate a random array */
int generate(int *a)
{
	int i, j, k, l;
//	int a[M]={0};
	int m=M;
	int flag=0;
	double start, finish;
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

	printf("\nRandom data generated, please see randam.txt for detail.\n");
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

/* put numbers of array a to b by insert sort method */
int sort(int *a)
{
	int i, j, temp;
	double start, finish;
	
	start = (double)clock();
	
	g[0] = a[0];

	for (i=1; i<M; i++) {
		j = i;
		g[j] = a[i];

		for ( ; (j>0) && (g[j]<g[j-1]); j--) {
			temp = g[j-1];
			g[j-1] = g[j];
			g[j] = temp;
		}
	}

	finish = (double)clock();
	spend = (finish-start)/CLOCKS_PER_SEC;

	return 0;
}

int main(void)
{
	int a[M] = {0};
	int b[M] = {0};
	int c[M] = {0};
	int l;
	FILE *fp1 = fopen("randam_sort.txt", "w+");
	FILE *fp2 = fopen("ascending_order_sort.txt", "w+");
	FILE *fp3 = fopen("equal_sort.txt", "w+");
	
/* randam generate */
	generate(a);

/* randam sort */
	sort(a);
	for(l=0; l<M; l++)
	{
//		printf("%d\t", g[l]);
		fprintf(fp1, "%d\t", g[l]);
	}
	fprintf(fp1, "\nrandam sort cost time is: %.4fs\n", spend);

	printf("\nRandam sort finished, please see randam_sort.txt for detail.\n");
	printf("Randam sort cost time: %.4fs\n", spend);

/* ascending order generate */
	for(l=0; l<M; l++){
		b[l] = l;
	}

/* ascending order data sort */
	sort(b);
	for(l=0; l<M; l++)
	{
		fprintf(fp2, "%d\t", g[l]);
	}
	fprintf(fp2, "\nascending order sort cost time is: %.4fs\n", spend);

	printf("\nAscending ordered data sort finished, please see ascending_order_sort.txt for detail.\n");
	printf("ascending order sort cost time: %.4fs\n", spend);

/* equal data generate */
	for(l=0; l<M; l++) {
		c[l] = 20;
	}

/* equal data sort */
	sort(c);
	for(l=0; l<M; l++)
	{
		fprintf(fp3, "%d\t", g[l]);
	}
	fprintf(fp3, "\nequal number sort cost time is: %.4fs\n", spend);

	printf("\nEqual number sort finished, please see equal_sort.txt for detail.\n");
	printf("equal numbet sort cost time: %.4fs\n", spend);

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fp1 = NULL;
	fp2 = NULL;
	fp3 = NULL;
	
	return 0;
}
