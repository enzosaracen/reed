#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Field Field;

struct Field {
	int	m, size;
	int	*log, *gol;
	int	gen;
};

int	_mult(Field *, int, int);
Field	*init(int);
int	mult(Field *, int, int);
