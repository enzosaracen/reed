#include "galois.h"

#define DIV8	0b11101
#define DIV4    0b11

typedef struct Reed Reed;
struct Reed {
	Field *f;
	int n, k, t;
	int *gen;
};

void gen(Reed *r)
{
	int i, j;

	#define A0 0

	#define GOL(v)	(r->f->gol[(v) % r->f->size])
	#define LOG(v)	(r->f->log[(v) % r->f->size])

	r->gen[0] = r->f->gol[A0];
	r->gen[1] = 1;
	for(i = 0; i < 2*r->t-1; i++) {
		for(j = i+1; j >= 0; j--)
			r->gen[j+1] = r->gen[j];
		r->gen[0] = 0;
		for(j = 0; j <= i+1; j++) {
			printf("%d\n", LOG(r->gen[j+1]) + i+A0+1);
			r->gen[j] += GOL(LOG(r->gen[j+1]) + i+A0+1);
			r->gen[j] %= r->f->size;
		}
		if(i == 2)
			break;
	}
}

int main(void)
{
	int i;
	Reed r;

	r.f = init(4, DIV4);
	r.n = r.f->size-1;
	r.k = 11;
	r.t = (r.n-r.k) / 2;
	r.gen = calloc(2*r.t + 1, sizeof(int));
	gen(&r);

	for(i = 2*r.t; i >= 0; i--)
		printf("%dx^%d%s", r.gen[i], i, (i == 0) ? "\n" : " + ");
	for(i = 0; i < r.f->size; i++)
		printf("(%d, %d)\n", i, r.f->gol[i]);
}
