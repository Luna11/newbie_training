/*-------------------------------------------------------
File name:
	gene_bit.c

Description:
	This file is used to generate M random datas between 1 to N with bitmap mathod

Author/Created Date:
	Luna Wang, 2014/07/24

Update history:
	none
------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define SHIFT 5
#define M 10
#define MASK 0x1F

int a[1+N/32]={0};
void set_bit(int *, int);
int get_bit(int *, int);

int main()
{
    int i, j;

	srand((int)time(0));

	for(i=1; i<=M; i++)
	{
		j=1+rand()%N;
		if(get_bit(a,j)==0)
		{
			set_bit(a,j);
			printf("%d\t", j);
		}
		else
		{
			i--;
			continue;
		}
	}
}

void set_bit(int *a, int n)
{
	a[n>>SHIFT]|=1<<(n&MASK);
}

int get_bit(int *a, int n)
{
	return a[n>>SHIFT]&1<<(n&MASK);
}
