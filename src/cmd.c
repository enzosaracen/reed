#include "raid.h"

int main(int argc, char **argv)
{
	int i, j, n, m;
	Reed *r;
	Disk **d;

	if(argc < 4) {
		fprintf(stderr, "usage: raid [n] [m] [all files]... [bad files]...\n");
		return 1;
	}
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	if(n <= 0 || m <= 0 || n+m > argc-3) {
		fprintf(stderr, "bad [n] and [m]\n");
		return 1;
	}
	d = malloc((n+m)*sizeof(Disk*));
	for(i = 0; i < n+m; i++) {
		d[i] = dnew(argv[3+i]);
		for(j = n+m; j < argc-3; j++)
			if(strcmp(argv[3+j], d[i]->name) == 0)
				d[i]->bad = 1;
	}
	r = reed(n, m, d);
	if(argc-3-n-m == 0) {
		for(i = r->n; i < r->n+r->m; i++)
			r->disk[i]->bad = 1;
		check(r);
	} else
		fix(r);
}
