/*
 * File name:
 *	count.c
 *
 * Copyright (C) 2014 Wistron
 *	All rights reserved.
 *
 * Description:
 *	Count the bit 1 number of an int number
 *
 * Author/Created Date:
 *	Luna Wang, Aug05'14
 *
 * Modification History:
 * 
 * Note:
 */

/* include files */
#include <stdio.h>

/* MACROS */
#define N 7

/* INTERAL FUNCTION */
int count(int m)
{
	int i=0;

	while(m)
	{
		if(m&1)
			i++;

		m>>=1;
	}

	return i;
}

/* IMPLEMENTATION */
int main()
{
	int q;
	//scanf("\nplease input an int number:%d\n", &n);
	q=count(N);
	printf("The number %d include %d bits 1.\n", N, q);

	return 0;

}
