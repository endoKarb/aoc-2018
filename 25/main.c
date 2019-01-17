//Advent of Code 2018 Day 25
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STAR 2048

typedef signed char dCoor;
typedef unsigned char uDist;
typedef unsigned short uIndex;
typedef unsigned short uSize;

typedef struct{
	dCoor vHead[4];
} tCoor;

typedef struct{
	uIndex uSeiza;
	tCoor sCoor;
} tStar;

typedef struct{
	uSize uSeiza;
	uSize uSize;
	tStar vHead[MAX_STAR];
} tStarList;

void
print_star(tStar sStar)
{
	printf("Seiza:%u, Coor:%d,%d,%d,%d\n", sStar.uSeiza,
		sStar.sCoor.vHead[0],
		sStar.sCoor.vHead[1],
		sStar.sCoor.vHead[2],
		sStar.sCoor.vHead[3]);
}

void
print_starlist(tStarList * pList)
{
	static tStar * pScan;
	for(int i = 0; i < pList -> uSize; ++i)
	{
		pScan = &pList -> vHead[i];
		print_star(*pScan);
	}
}

void
add_star(tStarList * pStarList, tStar sStar)
{
	pStarList -> vHead[pStarList -> uSize] = sStar;
	++pStarList -> uSize;
}

void
parse_input(tStarList * pStarList)
{
	static tStar sBuf;

	sBuf.uSeiza = 0;
	while(scanf("%hhd,%hhd,%hhd,%hhd\n",
			&sBuf.sCoor.vHead[0],
			&sBuf.sCoor.vHead[1],
			&sBuf.sCoor.vHead[2],
			&sBuf.sCoor.vHead[3]) == 4)
	{
		add_star(pStarList, sBuf);
	}
}

uDist
man_dist(tCoor p1, tCoor p2)
{
	dCoor d1;
	dCoor d2;
	uDist total = 0;

	for(int i = 0; i < 4; ++i)
	{
		d1 = p1.vHead[i];
		d2 = p2.vHead[i];
		total += abs(d1 - d2);
	}

	return total;
}

bool
same_seiza(tStar * p1, tStar * p2)
{
	if(man_dist(p1 -> sCoor, p2 -> sCoor) <= 3)
		return true;
	else
		return false;
}

void
find_adj(tStarList * pStarList, tStar * pStar)
{
	//print_star(*pStar);
	static tStar * pScan;
	for (int i = 0; i < pStarList -> uSize; ++i)
	{
		pScan = &pStarList -> vHead[i];
		if(pScan -> uSeiza != pStar -> uSeiza && same_seiza(pScan, pStar))
		{
			pScan -> uSeiza = pStar -> uSeiza;
			//print_star(*pScan);
			find_adj(pStarList, pScan);
		}
	}
	//putchar('\n');
}

void
funct(tStarList * pStarList)
{
	static tStar * pScan;
	for(int i = 0; i < pStarList -> uSize; ++i)
	{
		pScan = &pStarList -> vHead[i];
		if(!pScan -> uSeiza)
		{
			++pStarList -> uSeiza;
			pScan -> uSeiza = pStarList -> uSeiza;
			find_adj(pStarList, pScan);
			//putchar('\n');
		}
	}
	print_starlist(pStarList);
}

int main(void)
{
	tStarList * pStarList = (tStarList *) calloc(1, sizeof(tStarList));
	parse_input(pStarList);
	funct(pStarList);

	printf("Total number of constellations: %hu\n", pStarList -> uSeiza);

/*
*/
	return 0;
}
