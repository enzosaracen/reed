#include "raid.h"

Reed *reed(int n, int m, Disk **disk)
{
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
	van(r);
	return r;
}

void swap(int n, int *r1, int *r2)
{
	while(n--) {
		r1[n] ^= r2[n];
		r2[n] ^= r1[n];
		r1[n] ^= r2[n];
	}
}

void add(Reed *r, int n, int *r1, int *r2, int m)
{
	while(n--)
		r1[n] ^= gmult(r->f, r2[n], m);
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

void inv(Reed *r, int **mat)
{
	int i, j;	

loop:
	for(i = 0; i < r->n; i++) {
		if(mat[i][i] == 0)
			for(j = 0; j < r->n; j++)
				if(mat[j][i] > 0) {
					add(r, 2*r->n, mat[i], mat[j], 1);
					break;
				}
		for(j = 0; j < r->n; j++)
			if(j != i && mat[j][i] != 0)
				add(r, 2*r->n, mat[j], mat[i], gdiv(r->f, mat[j][i], mat[i][i]));
		if(mat[i][i] != 1)
			add(r, 2*r->n, mat[i], mat[i], gdiv(r->f, 1^mat[i][i], mat[i][i]));
	}
	for(i = 0; i < r->n; i++)
		for(j = 0; j < r->n; j++) {
			if(i == j) {
				if(mat[i][i] != 1)
					goto loop;
			} else if(mat[i][j] != 0)
				goto loop;
		}
}

void check(Reed *r)
{
	int i, j, k, *c;

	c = malloc(r->n*sizeof(int));
	for(i = 0; i < r->n; i++) {
		if(r->disk[i]->bad) {
			fprintf(stderr, "cannot make check with missing data\n");
			exit(1);
		}
		dopen(r->disk[i], "rb");
	}
	k = 0;
	for(i = r->n; i < r->n+r->m; i++)
		if(r->disk[i]->bad) {
			k = 1;
			dopen(r->disk[i], "wb");
		}
	if(k == 0)
		return;
	for(;;) {
		for(i = 0; i < r->n; i++) {
			c[i] = dget(r->disk[i]);
			if(c[i] == EOF)
				goto end;
		}
		for(i = r->n; i < r->n+r->m; i++) {
			if(!r->disk[i]->bad)
				continue;
			k = 0;
			for(j = 0; j < r->n; j++)
				k ^= gmult(r->f, r->van[i][j], c[j]);
			dput(r->disk[i], k);
		}
	}
end:
	for(i = r->n; i < r->n+r->m; i++)
		if(r->disk[i]->bad) {
			r->disk[i]->bad = 0;
			dflush(r->disk[i]);
		}
	free(c);
}

void fix(Reed *r)
{
	int i, j, k, **mat, *c;
	Disk **good;

	j = 0;
	good = malloc(sizeof(Disk*)*r->n);
	mat = malloc(sizeof(int*)*r->n);
	for(i = 0; i < r->n+r->m; i++)
		if(!r->disk[i]->bad) {
			good[j] = r->disk[i];
			dopen(good[j], "rb");
			mat[j] = calloc(2*r->n, sizeof(int));
			for(k = 0; k < r->n; k++)
				mat[j][k] = r->van[i][k];
			if(++j >= r->n)
				break;
		}
	if(j < r->n) {
		fprintf(stderr, "errors exceed %d\n", r->m);
		exit(1);
	}
	for(i = 0; i < r->n; i++) {
		if(r->disk[i]->bad)
			dopen(r->disk[i], "wb");
		mat[i][r->n+i] = 1;
	}
	inv(r, mat);

	c = malloc(sizeof(int)*r->n);
	for(;;) {
		for(i = 0; i < r->n; i++) {
			c[i] = dget(good[i]);
			if(c[i] == EOF)
				goto end;
		}
		for(i = 0; i < r->n; i++) {
			if(!r->disk[i]->bad)
				continue;
			k = 0;
			for(j = 0; j < r->n; j++)
				k ^= gmult(r->f, mat[i][j+r->n], c[j]);
			dput(r->disk[i], k);
		}
	}
end:
	for(i = 0; i < r->n; i++)
		if(r->disk[i]->bad) {
			r->disk[i]->bad = 0;
			dflush(r->disk[i]);
		}
	free(mat);
	free(c);
	free(good);
	check(r);
}
