#include "rain.h"

#define N	3
#define M	3

int main(void)
{
	int i;
	Reed *r;
	Disk *d[N+M];
	char name[] = "test/d0";

	for(i = 0; i < N+M; i++) {
		d[i] = dnew(strdup(name));
		name[N+M]++;
	}
	r = reed(N, M, d);
	check(r);
}
