#include "galois.h"

#define PRIM4	0b11
#define PRIM8	0b11101

typedef struct Reed Reed;

struct Reed {
	Field *f;
	int n, k, t;
	int *gen, *msg;
};

void gen(Reed *r)
{
	int i, j;

	r->gen[0] = r->f->gol[0];
	r->gen[1] = 1;
	for(i = 1; i < 2*r->t; i++) {
		for(j = i; j >= 0; j--)
			r->gen[j+1] = r->gen[j];
		r->gen[0] = 0;
		for(j = 0; j <= i; j++)
			r->gen[j] ^= gmult(r->f, r->gen[j+1], r->f->gol[i]);
	}
}

Reed *reed(int m, int k, int prim)
{
	Reed *r;

	r = malloc(sizeof(Reed));
	r->f = ginit(m, prim);
	r->n = r->f->size-1;
	r->k = k;
	r->t = (r->n-r->k) / 2;
	r->gen = calloc(2*r->t + 1, sizeof(int));
	r->msg = malloc((r->k + 2*r->t)*sizeof(int));
	gen(r);
	return r;
}

void encode(Reed *r, int *msg)
{
	int i, j, t;

	for(i = 0; i < r->k; i++)
		r->msg[(r->k + 2*r->t)-i-1] = msg[i];
	for(i = r->k + 2*r->t - 1; i >= 2*r->t; i--) {
		t = gdiv(r->f, r->msg[i], r->gen[2*r->t]);
		for(j = 2*r->t; j >= 0; j--)
			r->msg[i - (2*r->t-j)] ^= gmult(r->f, r->gen[j], t);
	}
	for(i = 0; i < r->k; i++)
		r->msg[(r->k + 2*r->t)-i-1] = msg[i];
}

int main(void)
{
	int i, msg[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	Reed *r;

	r = reed(4, 11, PRIM4);
	encode(r, msg);
	for(i = 2*r->t+r->k-1; i >= 0; i--)
		printf("%d ", r->msg[i]);
	printf("\n");
}
