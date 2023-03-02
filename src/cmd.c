#include "raid.h"

#define N	5
#define M	5

int main(void)
{
	int i, j;
	Reed *r;
	Disk *d[N+M];
	char name[16];

	for(i = 0; i < N+M; i++) {
		snprintf(name, sizeof(name), "test/%d", i);
		d[i] = dnew(strdup(name));
	}
	r = reed(N, M, d);
	for(i = 0; i < N+M; i++) {
		for(j = 0; j < N; j++)
			printf("%d\t", r->van[i][j]);
		printf("\n");
	}
	check(r);
}
