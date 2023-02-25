#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define GF	8
#define PR	0b11101
#define	READ	1
#define WRITE	2

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
	int	n, mode;
};

/*
 *	galois.c
 */
Field	*ginit(int, int);
int	ginv(Field *, int);
int	gmult(Field *, int, int);
int	gdiv(Field *, int, int);

/*
 *	reed.c
 */
void	swap(int, int *, int *);
void	van(Reed *);
Reed	*reed(int, int, Disk **);
void	check(Reed *);

/*
 *	disk.c
 */
Disk	*dnew(char *);
void	dopen(Disk *, int);
void	dput(Disk *, char);
void	dflush(Disk *);
int	dget(Disk *);
