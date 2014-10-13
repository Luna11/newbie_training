/*---------------------------------------------
Filename:
	qsort.c

Copyright (C) 2014 Wistron
	All rights reserved.

Description:
	This file is a misc char device driver and ioctl function file.

Author/Created Date:
	Luna Wang, Oct13'14

Modification History:

Note:

--------------------------------------------*/

#include <stdio.h>

void qsort(int*, int, int);

int main()
{
	int a[10] = {22, 9, 78, 2, 55, 12, 92, 43, 18, 80};
	int i;

	int l = 0, u = 9;
	
	qsort(a, l, u);

	printf("\nqsort result is:");
	for(i = 0; i < 10; i++){
		printf("%d\t", a[i]);
	}

	return 0;
}

void qsort(int *a, int l, int u)
{
	if(l >= u)
		return;
	int x = a[l];
	int i = l;
	int j = u;
	int f = 0;    //头部有个坑可以放置数据
	int r = 1;    //尾部没有坑，不可以放置数据
	int m;

	while(i < j){

		if(f == 0){
			while((a[j] >= x) && (j > i)){
				--j;
			}

			if(a[j] < x){
				a[i] = a[j];    //将尾部a[j]放到了头部的坑a[i]
				r = 0;	        //所以，尾部有坑了，是a[j]
//				f = 1;          //头部坑被占用
			}
		}

		if(r == 0){
			while((a[i] <= x) && (i < j)){
				++i;
			}

			if(a[i] > x){
				a[j] = a[i];
				f = 0;
//				r = 1;
			}
		}
	
	}
	a[i] = x;
	m = i;
	printf("\nm=%d, a[i]=%d", m, x);
	
	//递归调用qsort方法
	qsort(a, l, (m-1));
	qsort(a, (m+1), u);
}
