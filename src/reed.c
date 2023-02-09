#include "galois.h"

int tab1[8][8], tab2[8][8];

int main(void)
{
	int i, j;
	Field *f;

	f = init(3);
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++) {
			tab1[i][j] = mult(f, i, j);
			tab2[i][j] = _mult(f, i, j);
		}

	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++)
			printf("%d ", tab1[i][j]);
		printf("\n");
	}
	tab2[4][5]++;
	printf("---------------\n");
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++)
			printf("%d ", tab2[i][j]);
		printf("\n");
	}
}
