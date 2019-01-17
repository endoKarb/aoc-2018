//Advent of Code 2018 Day 25
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef signed char dCoor;
typedef unsigned char uDist;
typedef unsigned char uSize;

typedef struct{
	dCoor vHead[4];
} tCoor;

typedef struct node tNode;

struct node{
	tCoor sStar;
	tNode * pNext;
	tNode * pPrev;
};

typedef struct{
	uSize uSize;
	tNode * pHead;
} tList;

bool
add_node(tList * pList, tCoor sStar)
{
	tNode * pNew;
	tNode * pScan = pList -> pHead;

	pNew = (tNode *) calloc(1, sizeof(tNode));
	if(pNew == NULL)
		return false;

	pNew -> sStar = sStar;

	if(pScan)
	{
		while(pScan -> pNext)
			pScan = pScan -> pNext;
		pNew -> pPrev = pScan;
		pScan -> pNext = pNew;
	}
	else
	{
		pList -> pHead = pNew;
	}

	return true;
}

void
remove_node(tList * pList, tNode * pNode)
{
	if(!pList -> pHead)
	{
		puts("EMPTY LIST, CAN'T REMOVE");
		return;
	}

	tNode * pPrev = pNode -> pPrev;
	tNode * pNext = pNode -> pNext;

	if(pPrev)
	{
		pPrev -> pNext = pNext;
	}
	else
	{
		pList -> pHead = pNext;
	}
	if(pNext)
	{
		pNext -> pPrev = pPrev;
	}

	free(pNode);
}

void
parse_input(tCoor * p1, tCoor * p2, tList * pList)
{
	p1 -> vHead[0] = 0;
	p1 -> vHead[1] = 0;
	p1 -> vHead[2] = 2;
	p1 -> vHead[3] = -2;

	p2 -> vHead[0] = -1;
	p2 -> vHead[1] = 2;
	p2 -> vHead[2] = 2;
	p2 -> vHead[3] = 0;

	if(!add_node(pList, *p1))
		puts("ERROR");
	if(!add_node(pList, *p2))
		puts("ERROR");
}

uDist
man_dist(tCoor * p1, tCoor * p2)
{
	dCoor d1;
	dCoor d2;
	uDist total = 0;

	for(int i = 0; i < 4; ++i)
	{
		d1 = p1 -> vHead[i];
		d2 = p2 -> vHead[i];
		total += abs(d1 - d2);
	}

	return total;
}

void
function()
{
/*
	take first start form list, put it as head of list i
*/
}

int main(void)
{
	tList * pList = (tList *) calloc(1, sizeof(tList));
	tCoor * p1 = (tCoor *) calloc(1, sizeof(tCoor));
	tCoor * p2 = (tCoor *) calloc(1, sizeof(tCoor));
	parse_input(p1, p2, pList);
	printf("%d\n", man_dist(p1, p2));

	return 0;
}
