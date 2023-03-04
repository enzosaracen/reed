#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define GF	8
#define PR	0b11101

typedef struct Field Field;
typedef struct Reed Reed;
typedef struct Disk Disk;

struct Field {
	int	m, size;
	int	*log, *gol;
	int	prim;
};

struct Reed {
	Field	*f;
	int	n, m;
	int	**van;
	Disk	**disk;
};

struct Disk {
	FILE	*f;
	char	buf[BUFSIZ], *p, *name;
	int	n, bad;
};

/*
 *	galois.c
 */
Field	*ginit(int, int);
int	ginv(Field *, int);
int	gmult(Field *, int, int);
int	gdiv(Field *, int, int);

/*
 *	disk.c
 */
Disk	*dnew(char *);
void	dopen(Disk *, char *);
void	dput(Disk *, char);
void	dflush(Disk *);
int	dget(Disk *);

/*
 *	reed.c
 */
Reed	*reed(int, int, Disk **);
void	swap(int, int *, int *);
void	add(Reed *, int, int *, int *, int);
void	van(Reed *);
void	inv(Reed *, int **);
void	check(Reed *);
void	fix(Reed *);
