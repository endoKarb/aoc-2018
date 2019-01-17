//Advent of Code 2018 Day 6
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAP_SIZE 512
#define ALPH_SIZE 58
#define MAX_DIST 10000

typedef struct{
	unsigned int v;
	unsigned char ch;
} Mandist;

typedef struct{
	unsigned int x;
	unsigned int y;
} Coord;

typedef struct{
	Coord l[ALPH_SIZE];
	unsigned int t;
} Clist;

typedef bool Map[MAP_SIZE][MAP_SIZE];

void
scan_input(Clist * clist)
{
	Coord * list = (*clist).l;
	for(int i = 0; scanf("%u, %u\n", &(list[i].x), &(list[i].y)) == 2; ++i)
		++(*clist).t;
}

void
print_coor(Coord * pc)
{
	printf("y: %u x: %u\n", pc->y, pc->x);
}

unsigned int
manhattan_dist(Coord * pc1, Coord * pc2)
{
	return abs(pc1->y - pc2->y) + abs(pc1->x - pc2->x);
}

bool
near_enough(Clist * pclist, Coord * pc)
{
	unsigned int cdist = 0;
	for(int i = 0; i < pclist->t; ++i)
	{
		cdist += manhattan_dist(&pclist->l[i], pc);
	}

	return cdist < MAX_DIST;
}

int main(void)
{

	Clist * pclist;
	pclist = (Clist *) calloc(1, sizeof(Clist));
	scan_input(pclist);

	unsigned int answer = 0;
	Coord coord;
	for(int i = 0; i < MAP_SIZE; ++i)
	{
		for(int j = 0; j < MAP_SIZE; ++j)
		{
			coord.y = i;
			coord.x = j;

			//find if near enough
			bool b;
			if(b = near_enough(pclist, &coord))
				++answer;
			putchar(b + '0');
		}
		putchar('\n');
	}

	printf("The safe zone is %d tiles big.\n", answer);

	return 0;
}
