#include "galois.h"

int _mult(Field *f, int v1, int v2)
{
	int i, r, mask;

	r = 0;
	for(i = 0; i < f->m; i++) {
		if(v1 & 1)
			r ^= (v2 << i);
		v1 >>= 1;
		if(v1 == 0)
			break;
	}
	mask = 1 << (f->m + f->m - 2);
	for(i = f->m-2; i >= 0; i--) {
		if(r & mask) {
			r &= ~mask;
			r ^= (f->div << i);
		}
		mask >>= 1;
	}
	return r;
}

Field *init(int m, int div)
{
	int i, p;
	Field *f;

	f = malloc(sizeof(Field));
	f->m = m;
	f->size = 1 << m;
	f->log = malloc(f->size*sizeof(int));
	f->gol = malloc(f->size*sizeof(int));
	f->div = div;
	f->gol[0] = 1;
	p = 2;
	for(i = 1; i < f->size-1; i++) {
		f->gol[i] = p;
		f->log[p] = i;
		p = _mult(f, p, 2);
	}
	return f;
}

int mult(Field *f, int v1, int v2)
{
	int r;

	if(v1 == 0 || v2 == 0)
		return 0;
	r = f->log[v1] + f->log[v2];
	if(r >= f->size-1)
		r -= f->size-1;
	return f->gol[r];
}
