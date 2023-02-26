#include "rain.h"

Disk *dnew(char *name)
{
	Disk *d;

	d = malloc(sizeof(Disk));
	d->n = d->mode = 0;
	d->name = name;
	d->f = NULL;
	d->p = d->buf;
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
	*d->p++ = c;
	if(d->p-d->buf >= BUFSIZ) {
		fwrite(d->buf, 1, BUFSIZ, d->f);
		d->p = d->buf;
	}
}

void dflush(Disk *d)
{
	fwrite(d->buf, 1, d->p-d->buf, d->f);
}

int dget(Disk *d)
{
	if(d->p-d->buf >= d->n) {
		d->n = fread(d->buf, 1, BUFSIZ, d->f);
		if(d->n <= 0)
			return EOF;
		d->p = d->buf;
	}
	return (uint8_t)*d->p++;
}
