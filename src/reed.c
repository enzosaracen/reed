#include "galois.h"

#define GF	8
#define PRIM	0b11101

typedef struct Reed Reed;
struct Reed {
	Field	*f;
	int	n, m;
	int	**van;
	FILE	**disk;
};

Reed *reed(int n, int m, FILE **disk)
{
	Reed *r;

	r = malloc(sizeof(Reed));
	r->f = ginit(GF, PRIM);
	if(n+m >= r->f->size) {
		fprintf(stderr, "disk count exceeds %d\n", r->f->size-1);
		exit(1);
	}
	r->n = n;
	r->m = m;
	r->disk = disk;
	r->van = NULL;
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

void van(Reed *r)
{
	int i, j;

	if(r->van != NULL)
		return;
	r->van = malloc((r->n+r->m)*sizeof(int*));
	for(i = 0; i < r->n+r->m; i++) {
		r->van[i] = malloc(r->n*sizeof(int));
		r->van[i][0] = i > 0 ? 1 : 0;
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
	}
}

void check(Reed *r)
{
	van(r);
}

int main(void)
{
	int i, j;
	Reed *r;
	FILE *d[6];
	char s[3];

	s[0] = 'd';
	s[1] = '0';
	s[2] = 0;
	for(i = 0; i < 6; i++) {
		d[i] = fopen(s, "w");
		s[1]++;
	}
	r = reed(3, 3, d);
	check(r);
	for(i = 0; i < r->n+r->m; i++) {
		for(j = 0; j < r->n; j++)
			printf("%d ", r->van[i][j]);
		printf("\n");
	}
}
