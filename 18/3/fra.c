//AoC 2018 Day 3
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE_SIDE 1024
#define SIZE_LIST 2048

typedef struct {
	int16_t map[SIZE_SIDE][SIZE_SIDE];
} Fabric;

typedef struct {
	uint16_t id;
	uint16_t coor[2];
	uint8_t size[2];
	bool overlap;
	unsigned int total;
} Line;

typedef struct {
	Line * list[SIZE_LIST];
	uint16_t count;
} List;

void
print_map(Fabric *);

bool
append_line(List * dest, Line * item);

void
scan_input(List *);
void
update_map(Fabric *, Line *);
void
compute_final_map(Fabric *, List *);
unsigned int
count_overlap(Fabric *);
unsigned int
find_nooverlap(List *);
bool
no_overlap(Line *, Fabric *);
void
count(List *, Fabric *);

int
main(void)
{
	Fabric * pfab = (Fabric *) calloc(1, sizeof(Fabric));
	List * plist = (List *) calloc(1, sizeof(List));

	scan_input(plist);

	compute_final_map(pfab, plist);
	//print_map(pfab);
	unsigned int answer = count_overlap(pfab);
	printf("%u square inches of fabric are within two or more claims.\n", answer);
	count(plist, pfab);
	unsigned int answer2 = find_nooverlap(plist);
	printf("id = %u doesn't overlap at all.\n", answer2);
	return 0;
}

void
print_map(Fabric * pfab)
{
	putchar('\n');
	unsigned int i, j;
	for(i = 0; i < SIZE_SIDE; ++i)
	{
		for(j = 0; j < SIZE_SIDE; ++j)
		{
			printf("%d", pfab->map[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
}

void
scan_input(List * plist)
{
	uint16_t id;
	uint16_t coor[2];
	uint8_t size[2];

	uint16_t i = 0;

	while(scanf("#%hu @ %hu,%hu: %hhux%hhu\n",
			&id, &coor[0], &coor[1], &size[0], &size[1]) == 5)
	{
		Line * pline = (Line *) calloc(1, sizeof(Line));

		pline->id = id;
		pline->coor[0] = coor[0];
		pline->coor[1] = coor[1];
		pline->size[0] = size[0];
		pline->size[1] = size[1];

		append_line(plist, pline);

	}
}

bool
append_line(List * plist, Line * pline)
{
	if(plist->count >= SIZE_LIST)
		return false;
	else
	{
		plist->list[plist->count] = pline;
		++plist->count;
		return true;
	}
}

void
update_map(Fabric * pmap, Line * pline)
{
	uint16_t voff = pline -> coor[1];
	uint16_t hoff = pline -> coor[0];

	for(int i = voff; i < pline->size[1] + voff; ++i)
	{
		for(int j = hoff; j < pline->size[0] + hoff; ++j)
		{
			if(pmap->map[i][j] == 0)
				pmap->map[i][j] = pline->id;
			else
				pmap->map[i][j] = -1;
		}
	}
}

void
compute_final_map(Fabric * pmap, List * plist)
{

	Line ** scan = plist->list;
	while(*scan)
	{
		update_map(pmap, *scan);
		++scan;
	}
}

unsigned int
count_overlap(Fabric * pfab)
{
	unsigned int o = 0;
	for(int i = 0; i < SIZE_SIDE; ++i)
	{
		for(int j = 0; j < SIZE_SIDE; ++j)
		{
			if(pfab->map[i][j] == -1)
			{
				++o;
			}
		}
	}
	return o;
}

unsigned int
find_nooverlap(List * plist)
{
	Line ** scan = plist->list;
	while(*scan)
	{
		unsigned int size = (*scan)->size[0] * (*scan)->size[1];
		if((*scan)->total == size)
			return (*scan)->id;
		++scan;
	}
	return 0;
}

void
count(List * plist, Fabric * pfab)
{
	for(int i = 0; i < SIZE_SIDE; ++i)
	{
		for(int j = 0; j < SIZE_SIDE; ++j)
		{
			if(pfab->map[i][j] > 0)
			{
				++plist->list[pfab->map[i][j] - 1]->total;
			}
		}
	}
}
