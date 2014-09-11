#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100
#define M 60

int main()
{
	int i, j, t, temp;
	int a[N] = {0};

	for (i = 0; i < N; i++) {
		a[i] = i;
	}

	srand((int)time(0));

	for (j = 0; j < M; j++) {
		t = rand() % N;
		temp = a[j];
		a[j] = a[t];
		a[t] = temp;
	}

	for (j = 0; j < M; j++) {
		printf("%d\t", a[j]);
	}
}
