#include "galois.h"

#define GF	8
#define PRIM	0b11101

typedef struct Reed Reed;
struct Reed {
	Field	*f;
	int	n, m;
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
	return r;
}

int main(void)
{
	int i;
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
}
