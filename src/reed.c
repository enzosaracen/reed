#include "galois.h"

#define DIV8	0b11101

typedef struct Reed Reed;
struct Reed {
	Field *f;
	int n, k, t;
	int *gen;
};

void gen(Reed *r)
{
	int i, j;

	#define A0 1

	r->gen[0] = r->f->gol[A0];
	r->gen[1] = 1;
	for(i = 0; i < 2*r->t-1; i++) {
		for(j = i+1; j >= 0; j--) {
			r->gen[j+1] = r->gen[j];
			r->gen[j] = 0;
			if(j == i+1)
				continue;
			r->gen[j+1] = (r->gen[j+1] +
				r->f->gol[(r->f->log[r->gen[j+2]] + i+A0+1) % (r->f->size-1)])
				% (r->f->size-1);
			if(j == 0)
				r->gen[j] = r->f->gol[(r->f->log[r->gen[j+1]] + i+A0+1) % (r->f->size-1)];
		}
		break;	
	}
}

int main(void)
{
	int i;
	Reed r;

	r.f = init(8, DIV8);
	r.n = r.f->size-1;
	r.k = 239;
	r.t = (r.n-r.k) / 2;
	r.gen = calloc(2*r.t + 1, sizeof(int));
	gen(&r);

	for(i = 2*r.t; i >= 0; i--)
		printf("%dx^%d%s", r.gen[i], i, (i == 0) ? "\n" : " + ");
}
