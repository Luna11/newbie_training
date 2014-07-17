//==========================================
// Filename:
//      sort_int.c
//
// Description:
//      This file is for weekly programming ex01, but not use bitmap. 
//
// Author/Created Date:
//      Luna Wang, 2014/07/17
//
// Update history:
//      none
//==========================================

#include <stdio.h>

#define N 35  //10000000
#define MAXLINE 9

int main()
{
	int a[N]={0};
    int i, k, n;
    char buf[MAXLINE];

	FILE *fp=fopen("./list.txt", "r");

//  read the data of list.txt, and set arr a[N].
	while(fgets(buf, MAXLINE, fp)!=NULL)
		{
			n=atoi(buf);

			for(i=0; i<N; i++)
				{
					if(i==n)
						{
							a[i]=1;
							break;
						}
				}
		}

//  put out result.
	for(k=0; k<N; k++)
		{
			if(a[k])
				printf("%d\t", k);
		}

	printf("\n");

//close list.txt
	fclose(fp);
	fp=NULL;

	return 0;
}
