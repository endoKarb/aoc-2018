//Advent of Code 2018 Day 17
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAP_SIZE (1 << 10)
#define MAX_DROPS (1 << 4)

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

typedef struct{
	unsigned int min;
	unsigned int max;
} tLimit;

typedef struct{
	char vHead[MAP_SIZE][MAP_SIZE];
} tMap;

typedef struct{
	unsigned int x;
	unsigned int y;
} tWell;

typedef struct drop tDrop;
struct drop{
	unsigned int x;
	unsigned int y;
	tDrop * pNext;
	tDrop * pPrev;
};

typedef struct{
	unsigned char size;
	tDrop vHead[MAX_DROPS];
	tDrop * pHead;
} tDropList;

typedef struct{
	tLimit sLimX;
	tLimit sLimY;
	tMap sMap;
	tDropList sDrops;
	tWell sWell;
} tState;

tState STATE;

void
change_tile(const unsigned int y, const unsigned int x, const char ch)
{
	STATE.sMap.vHead[y][x] = ch;
}

tDrop *
last_drop(void)
{
	static tDrop * pScan;
	pScan = STATE.sDrops.pHead;

	while(pScan && pScan -> pNext)
	{
		pScan = pScan -> pNext;
	}

	return pScan;
}

tDrop *
first_drop(void)
{
	static tDrop * pScan;

	pScan = STATE.sDrops.pHead;

	while(pScan && pScan -> pPrev)
	{
		pScan = pScan -> pPrev;
	}

	return pScan;
}

void
add_drop(const unsigned int y, const unsigned int x)
{
	static tDrop * pLast;
	tDrop * pNew = (tDrop *) calloc(1, sizeof(tDrop));

	pNew -> y = y;
	pNew -> x = x;

	pLast = last_drop();
	pNew -> pPrev = pLast;

	if(pLast)
	{
		pLast -> pNext = pNew;
	}
	else
	{
		STATE.sDrops.pHead = pNew;
	}
/*
	printf("new: %p, new-prev: %p, new-next: %p\n",
		pNew, pNew -> pPrev, pNew -> pNext);
	if(pLast)
	{
		printf("pLast: %p, pLast-prev: %p, pLast-next: %p\n",
		pLast, pLast -> pPrev, pLast -> pNext);
	}
*/

}

void
parse_input()
{
	STATE.sWell.x = 500;
	change_tile(STATE.sWell.y, STATE.sWell.x, '+');
	add_drop(STATE.sWell.y, STATE.sWell.x);

	STATE.sLimY.min = 0;
	STATE.sLimY.max = 7;

	STATE.sLimX.min = 495;
	STATE.sLimX.max = 505;


	//manually adding some clay
	change_tile(4, 498, '#');
	change_tile(4, 499, '#');
	change_tile(4, 500, '#');
	change_tile(4, 501, '#');
	change_tile(4, 502, '#');

	change_tile(3, 498, '#');
	change_tile(3, 502, '#');
}

void
print_map()
{
	unsigned int minY = STATE.sLimY.min;
	unsigned int maxY = STATE.sLimY.max;
	unsigned int minX = STATE.sLimX.min;
	unsigned int maxX = STATE.sLimX.max;

	for(int i = minY; i < maxY; ++i)
	{
		for(int j = minX; j < maxX; ++j)
		{
			putchar(STATE.sMap.vHead[i][j]);
		}
		putchar('\n');
	}
}

char
get_tile(const unsigned int y, const unsigned int x)
{
	return STATE.sMap.vHead[y][x];
}


void
remove_drop(tDrop * pDrop)
{
	printf("pDrop: %p, pDrop-prev: %p, pDrop-next: %p\n",
		pDrop, pDrop -> pPrev, pDrop -> pNext);

	if(pDrop -> pNext)
	{
		pDrop -> pNext -> pPrev = pDrop -> pPrev;
	}

	if(pDrop -> pPrev)
	{
		pDrop -> pPrev -> pNext = pDrop -> pNext;
	}
	else
	{
		STATE.sDrops.pHead = first_drop();
	}

	free(pDrop);
}

void
move_drop(tDrop * pDrop, unsigned char dir)
{
	static unsigned int y, x;

	y = pDrop -> y;
	x = pDrop -> x;

	switch(dir)
	{
		case UP:
			--y;
			break;
		case LEFT:
			--x;
			break;
		case RIGHT:
			++x;
			break;
		case DOWN:
			++y;
			break;
		default:
			puts("INVALID DIRECTION");
	}

	pDrop -> y = y;
	pDrop -> x = x;
	change_tile(pDrop -> y, pDrop -> x, '|');
}

bool
check_free(tDrop * pDrop, char dir)
{
	char buf;
	switch(dir)
	{
		case DOWN:
			buf = get_tile(pDrop -> y + 1, pDrop -> x);
			break;
		case LEFT:
			buf = get_tile(pDrop -> y, pDrop -> x - 1);
			break;
		case RIGHT:
			buf = get_tile(pDrop -> y, pDrop -> x + 1);
			break;
		case UP:
			buf = get_tile(pDrop -> y - 1, pDrop -> x);
			break;
		default:
			buf = '0';
			puts("INVALID DIR");
	}
	if(buf == '.' || (buf == '|' && dir == UP))
		return true;
	else
		return false;
}

void
tick()
{
	puts("tick");

	static tDrop * pScan;
	pScan = STATE.sDrops.pHead;

	if(!pScan)
	{
		puts("empty queue");
	}
	else
	{
		while(pScan)
		{
			//printf("%p\n", pScan);
			if(check_free(pScan, DOWN))
			{
				puts("DOWN");
				move_drop(pScan, DOWN);
			}
			else if(!(check_free(pScan, LEFT) || check_free(pScan, RIGHT)) && check_free(pScan, UP))
			{
				puts("UP");
				move_drop(pScan, UP);
			}
			else
			{
				puts("SIDE");
				if(check_free(pScan, LEFT))
				{
					add_drop(pScan -> y, pScan -> x);
					move_drop(last_drop(), LEFT);
					puts("LEFT");
				}
				if(check_free(pScan, RIGHT))
				{
					add_drop(pScan -> y, pScan -> x);
					move_drop(last_drop(), RIGHT);
					puts("RIGHT");
				}
				remove_drop(pScan);
			}
			pScan = pScan -> pNext;
			print_map();
		}
	}
}

int main(void)
{
	memset(STATE.sMap.vHead, '.', sizeof(tMap));
	parse_input();

	print_map();
	tick();
	tick();
	tick();
	tick();
	tick();
	tick();

	return 0;
}
