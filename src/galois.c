#include "raid.h"

int _gmult(Field *f, int v1, int v2)
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
			r ^= (f->prim << i);
		}
		mask >>= 1;
	}
	return r;
}

Field *ginit(int m, int prim)
{
	int i, p;
	Field *f;

	f = malloc(sizeof(Field));
	f->m = m;
	f->size = 1 << f->m;
	f->log = malloc(f->size*sizeof(int));
	f->gol = malloc(f->size*sizeof(int));
	f->prim = prim;
	f->gol[0] = 1;
	p = 2;
	for(i = 1; i < f->size-1; i++) {
		f->gol[i] = p;
		f->log[p] = i;
		p = _gmult(f, p, 2);
	}
	return f;
}

int ginv(Field *f, int v)
{
	if(v == 0)
		return 0;
	return f->gol[(f->size-1 - f->log[v]) % (f->size-1)];
}

int gmult(Field *f, int v1, int v2)
{
	int r;

	if(v1 == 0 || v2 == 0)
		return 0;
	r = f->log[v1] + f->log[v2];
	if(r >= f->size-1)
		r -= f->size-1;
	return f->gol[r];
}

int gdiv(Field *f, int v1, int v2)
{
	return gmult(f, v1, ginv(f, v2));
}
