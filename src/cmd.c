#include "raid.h"

#define N	5
#define M	5

int main(void)
{
	int i;
	Reed *r;
	Disk *d[N+M];
	char name[16];

	for(i = 0; i < N+M; i++) {
		snprintf(name, sizeof(name), "test/%d", i);
		d[i] = dnew(strdup(name));
	}
	r = reed(N, M, d);
	check(r);
	for(i = N; i < N+M; i++)
		dopen(d[i], "rb");
	for(i = 0; i < M; i++)
		r->disk[i]->bad = 1;
	fix(r);
}
