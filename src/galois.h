#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Field Field;

struct Field {
	int	m, size;
	int	*log, *gol;
	int	prim;
};

Field	*ginit(int, int);
int	ginv(Field *, int);
int	gmult(Field *, int, int);
int	gdiv(Field *, int, int);
