//Advent of Code 2018 Day 6
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAP_SIZE 512
#define ALPH_SIZE 64

typedef struct{
	unsigned int v;
	unsigned char ch;
} MANDIST;

typedef struct{
	unsigned int x;
	unsigned int y;
} CRD;

typedef struct{
	CRD l[ALPH_SIZE];
	unsigned int t;
	int size[ALPH_SIZE];
} CLIST;

typedef struct{
	CRD crd;
	char ch;
} MAP_CELL;

typedef char MAP_LINE[MAP_SIZE];

CLIST * PCL;

void
scan_input(CLIST * pcl)
{
	for
	(
		int i = 0;
		scanf("%u, %u\n", &(pcl->l[i].x), &(pcl->l[i].y)) == 2;
		++i
	)
		++pcl->t;
}

void
print_coor(CRD * pc)
{
	printf("y: %u x: %u\n", pc->y, pc->x);
}

unsigned int
manhattan_dist(const CRD * pc1, const CRD * pc2)
{
	unsigned int d = abs(pc1->y - pc2->y) + abs(pc1->x - pc2->x);
	return d;
}

int
smaller(const void * p1, const void * p2)
{
	const unsigned int * u1 = (unsigned int *) p1;
	const unsigned int * u2 = (unsigned int *) p2;

	return *u1 - *u2;
}

bool
is_border(CRD * pc)
{
	if(pc->y == 0 || pc->x == 0 ||
	pc->y == MAP_SIZE-1 || pc->x == MAP_SIZE-1)
		return true;

	return false;

}

void
range_map(MAP_LINE * pmap, char (*f)(const MAP_CELL *))
{
	MAP_CELL m;
	for(int i = 0; i < MAP_SIZE; ++i)
	{
		for(int j = 0; j < MAP_SIZE; ++j)
		{
			m.crd.y = i;
			m.crd.x = j;
			m.ch = *(pmap[i] + j);

			*(pmap[i] + j) = f(&m);
		}
	}
}

char
print(const MAP_CELL * pcell)
{
	putchar(pcell->ch);
	if(pcell->crd.x >= MAP_SIZE - 1)
		putchar('\n');
	return pcell->ch;
}

char
find_letter(const int i)
{
	char r = i + 'A';
	if(r  > 'Z')
		r += ('a' - 'Z' - 1);

	if(isalpha(r))
		return r;
	else
	{
		puts("nonalpha!");
		return '?';
	}
}

void
count_size(const char ch)
{
	++PCL->size[ch - 'A'];
}

char
find_nearest(const MAP_CELL * pcell)
{
	static MANDIST m[ALPH_SIZE];

	for (int i = 0; i < PCL->t; ++i)
	{
		m[i].ch = find_letter(i); 
		m[i].v = manhattan_dist(&PCL->l[i], &pcell->crd);
	}

	qsort(m, PCL->t, sizeof(m[0]), smaller);

	count_size(m[0].ch);

	if(m[0].v == m[1].v)
		return '.';
	else
		return m[0].ch;
}

int main(void)
{

	MAP_LINE * pmap = (MAP_LINE *) calloc(MAP_SIZE, sizeof(MAP_LINE));

	PCL = (CLIST *) calloc(1, sizeof(CLIST));
	scan_input(PCL);

	range_map(pmap, find_nearest);
	//range_map(pmap, print);

	for(int i = 0; i < ALPH_SIZE; ++i)
	{	
		printf("%c %d\n", i + 'A', PCL->size[i]);
	}

	qsort(PCL->size, ALPH_SIZE, sizeof(PCL->size[0]), smaller);

	printf("The answer is: %d\n", PCL->size[ALPH_SIZE - 1]);

	return 0;
}
