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
		r->msg[2*r->t+i] = msg[i];
	for(i = r->k+2*r->t-1; i >= 2*r->t; i--) {
		t = gdiv(r->f, r->msg[i], r->gen[2*r->t]);
		for(j = 2*r->t; j >= 0; j--)
			r->msg[i - (2*r->t-j)] ^= gmult(r->f, r->gen[j], t);
	}
	for(i = 0; i < r->k; i++)
		r->msg[2*r->t+i] = msg[i];
}

int *berlekamp(Reed *r, int *s)
{
	int *c, d, *pc, *tmp, pd, l, n, m, i;

	c = calloc(2*r->t, sizeof(int));
	pc = calloc(2*r->t, sizeof(int));
	tmp = calloc(2*r->t, sizeof(int));
	l = 0;
	c[0] = pc[0] = pd = m = 1;

	for(n = 0; n < 2*r->t; n++) {
		d = s[n];
		for(i = 1; i <= l; i++)
			d ^= gmult(r->f, s[n-m], c[m]);
		if(d == 0)
			m++;
		else if(n >= 2*l) {
			for(i = 0; i < 2*r->t; i++)
				tmp[i] = c[i];
			for(i = 0; i < 2*r->t-m; i++)
				c[i+m] ^= gmult(r->f, gdiv(r->f, d, pd), pc[i]);
			for(i = 0; i < 2*r->t; i++)
				pc[i] = tmp[i];
			l = n+1 - l;
			pd = d;
			m = 1;
		} else {
			for(i = 0; i < 2*r->t-m; i++)
				c[i+m] ^= gmult(r->f, gdiv(r->f, d, pd), pc[i]);
			m++;
		}
	}
	free(pc);
	free(tmp);
	return c;
}

void decode(Reed *r)
{
	int *s, i, j, *c;

	s = malloc(2*r->t*sizeof(int));
	for(i = 0; i < 2*r->t; i++) {
		s[i] = r->msg[r->n];
		for(j = r->n-1; j >= 0; j--) {
			s[i] = gmult(r->f, s[i], r->f->gol[i]);
			s[i] ^= r->msg[j];
		}
	}
	c = berlekamp(r, s);
	printf("berlekamp: ");
	for(i = 0; i < 2*r->t; i++)
		printf("%d ", c[i]);
	printf("\n");
}

int main(void)
{
	int i, msg[11] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	Reed *r;

	r = reed(4, 11, PRIM4);
	encode(r, msg);
	printf("sent:\t\t");
	for(i = 2*r->t+r->k-1; i >= 0; i--)
		printf("%d ", r->msg[i]);
	printf("\n");
	r->msg[8] = 11;
	printf("received:\t");
	for(i = 2*r->t+r->k-1; i >= 0; i--)
		printf("%d ", r->msg[i]);
	printf("\n");
	decode(r);
}
