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
	int q, n;
	printf("Please input a number:");
	scanf("%d", &n);
	q=count(n);
	printf("The number %d include %d bits 1.\n", n, q);

	return 0;
}
