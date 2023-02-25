#include "rain.h"

Disk *dnew(char *name)
{
	Disk *d;

	d = malloc(sizeof(Disk));
	d->n = 0;
	d->name = name;
	d->f = NULL;
	d->mode = 0;
	return d;
}

void dopen(Disk *d, int mode)
{
	if(d->f != NULL)
		fclose(d->f);
	d->f = fopen(d->name, mode == READ ? "rb" : "wb");
	if(d->f == NULL) {
		fprintf(stderr, "cannot open file '%s'\n", d->name);
		exit(1);
	}
	d->mode = mode;
}

void dput(Disk *d, char c)
{
	if(d->mode != WRITE)
		return;
	if(d->n <= 0) {
		d->n = BUFSIZ;
		d->p = d->buf;
	}
	*d->p++ = c;
	d->n--;
	if(d->n == 0)
		fwrite(d->buf, 1, BUFSIZ, d->f);
}

void dflush(Disk *d)
{
	fwrite(d->buf, 1, BUFSIZ-d->n-1, d->f);
	d->n = 0;
}

int dget(Disk *d)
{
	if(d->mode != READ)
		return EOF;
	if(d->n <= 0) {
		d->n = fread(d->buf, 1, BUFSIZ, d->f);
		if(d->n <= 0)
			return EOF;
		d->p = d->buf;
	} else {
		d->n--;
		d->p++;
	}
	return (uint8_t)*d->p;
}
