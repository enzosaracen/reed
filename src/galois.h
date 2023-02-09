#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Field Field;

struct Field {
	int	m, size;
	int	*log, *gol;
	int	div;
};

Field	*init(int, int);
int	mult(Field *, int, int);
