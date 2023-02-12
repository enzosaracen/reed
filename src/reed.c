#include "galois.h"

#define DIV8	0b11101
#define DIV4    0b11

typedef struct Reed Reed;
struct Reed {
	Field *f;
	int n, k, t;
	int *gen;
};

void prgen(Reed *r)
{
	int i, start;

	start = 0;
	for(i = 2*r->t; i >= 0; i--) {
		if(!start && r->gen[i] != 0)
			start = 1;
		if(start)
			printf("%d%s", r->gen[i], (i == 0) ? "\n" : " + ");
	}
}

void gen(Reed *r)
{
	int i, j;

	r->gen[0] = r->f->gol[0];
	r->gen[1] = 1;
	for(i = 1; i < 2*r->t; i++) {
		prgen(r);
		printf("* (x + %d)\n\n", r->f->gol[i]);
		for(j = i; j >= 0; j--)
			r->gen[j+1] = r->gen[j];
		r->gen[0] = 0;
		for(j = 0; j <= i; j++)
			r->gen[j] ^= r->f->gol[(r->f->log[r->gen[j+1]]+i)%r->f->size];
	}
	prgen(r);
}

int main(void)
{
	Reed r;

	r.f = init(8, DIV8);
	r.n = r.f->size-1;
	r.k = 239;
	r.t = (r.n-r.k) / 2;
	r.gen = calloc(2*r.t + 1, sizeof(int));
	gen(&r);
}
