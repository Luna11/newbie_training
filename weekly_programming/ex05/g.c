/*
 * File name:
 *	g.c
 *
 * Description:
 *	generate random numbers
 *
 * Copyright (C) 2014 Wistron
 *	All rights reserved.
 *
 * Author/Created date
 *	Luna Wang, Sep11'14
 *
 * Modify history
 *
 * Note
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000000
#define M 9000000

int main()
{
	int i, j;
	int m = M;
	FILE *fp = fopen("generate1.txt", "w+");

	srand((int)time(0));

	for(i = 0; i < N; i++) {
		j = rand() % (N - i);
		if (j < m) {
			printf("%d\t", i);
			fprintf(fp, "%d\t", i);
			m--;
		}

	}

	fclose(fp);
	fp = NULL;
	return 0;
}
