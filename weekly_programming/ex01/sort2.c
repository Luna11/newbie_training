//==========================================
// Filename:
// 		sort2.c
//
// Description:
// 		This file is used to sort with bitmap
//
// Author/Created Date:
// 		Luna Wang, 2014/07/16
//
// Update history:
//      none
//==========================================

#include <stdio.h>

#define BITPERWORD 32   //per int place 32 bits
#define SHIFT 5
#define MASK 0x1F
#define MAXLINE 9       //one num max is 8 char, but add '\0'
#define N 10000000 

void set_bit(int *, int);
int get_bit(int *, int);

int main()
{
		int a[N/BITPERWORD+1]={0};
		int i, n;
		char buf[MAXLINE];
		FILE *fp=fopen("./list.txt", "r");

		if(fp){
    			printf("list.txt opened ok\n");
				while(fgets(buf, MAXLINE, fp) != NULL){
					 n = atoi(buf);
              		set_bit(a, n);
					printf("%dset ", n);
				}
		}


		for(i=1; i<=N; i++){  
						
				if(get_bit(a, i)){
						printf("%d\n", i);
				}
				else {
						printf("no data %d\n", i);
//						break;
				}
		}


		fclose(fp);
		fp = NULL;
		return 0; 

}

void set_bit(int *a, int n)
{
	a[n>>SHIFT] |= (1<<(n & MASK)); 
}

int get_bit(int *a, int n)
{
	return a[n>>SHIFT] & (1<<(n & MASK));
}
