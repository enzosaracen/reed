#include "rain.h"

void swap(int n, int *r1, int *r2)
{
	while(n--) {
		r1[n] ^= r2[n];
		r2[n] ^= r1[n];
		r1[n] ^= r2[n];
	}
}

void van(Reed *r)
{
	int i, j, k, t;

	r->van = malloc((r->n+r->m)*sizeof(int*));
	for(i = 0; i < r->n+r->m; i++) {
		r->van[i] = malloc(r->n*sizeof(int));
		r->van[i][0] = 1;
		for(j = 1; j < r->n; j++)
			r->van[i][j] = gmult(r->f, r->van[i][j-1], i);
	}
	for(i = 0; i < r->n; i++) {
		if(r->van[i][i] == 0) {
			for(j = i+1; j < r->n+r->m; j++)
				if(r->van[j][i] != 0)
					break;
			swap(r->n, r->van[i], r->van[j]);
		}
		t = ginv(r->f, r->van[i][i]);
		for(j = 0; j < r->n+r->m; j++)
			r->van[j][i] = gmult(r->f, r->van[j][i], t);
		for(j = 0; j < r->n; j++) {
			t = r->van[i][j];
			if(j == i || r->van[i][j] == 0)
				continue;
			for(k = 0; k < r->n+r->m; k++)
				r->van[k][j] ^= gmult(r->f, r->van[k][i], t);
		}
	}
}

Reed *reed(int n, int m, Disk **disk)
{
	int i;
	Reed *r;

	r = malloc(sizeof(Reed));
	r->f = ginit(GF, PR);
	if(n+m >= r->f->size) {
		fprintf(stderr, "disk count exceeds %d\n", r->f->size-1);
		exit(1);
	}
	r->n = n;
	r->m = m;
	r->disk = disk;
	for(i = 0; i < r->n; i++)
		dopen(disk[i], READ);
	for(i = r->n; i < r->n+r->m; i++)
		dopen(disk[i], WRITE);
	van(r);
	return r;
}

void check(Reed *r)
{
	int i, j, *c, s;

	c = malloc(r->n*sizeof(int));
	for(;;) {
		for(i = 0; i < r->n; i++) {
			c[i] = dget(r->disk[i]);
			if(c[i] == EOF) {
				for(i = r->n; i < r->n+r->m; i++)
					dflush(r->disk[i]);
				free(c);
				return;
			}
		}
		for(i = r->n; i < r->n+r->m; i++) {
			s = 0;
			for(j = 0; j < r->n; j++)
				s ^= gmult(r->f, r->van[i][j], c[j]);
			dput(r->disk[i], s);
		}
	}
}
