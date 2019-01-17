//Advent of Code 2018 Day 15
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAP_SIZE (1 << 5)
#define QUEUE_SIZE (1 << 12)

typedef struct{
	int x;
	int y;
} tCoor;

typedef struct{
	unsigned int start;
	unsigned int end;
	tCoor q[QUEUE_SIZE];
} tQueue;

typedef union{
	char map[MAP_SIZE][MAP_SIZE];
	short dist_map[MAP_SIZE][MAP_SIZE];
} uMap;

typedef struct{
	uMap * pMap;
	tQueue * pQ;
} tState;

tState STATE;

void
print_map(uMap * pMap)
{
	for(int i = 0; i < MAP_SIZE; ++i)
	{
		for(int j = 0; j < MAP_SIZE; ++j)
		{
			char ch;
			switch(pMap -> dist_map[i][j])
			{
			case -1:
				ch = '.';
				break;
			default:
				ch = pMap -> dist_map[i][j] + '0';
				//ch = 'X';
			}
			putchar(ch);
		}
		putchar('\n');
	}
}

void
queue_push(tCoor * pCoor)
{
	//warning
	if(STATE.pQ -> end == STATE.pQ -> start)
	{
		puts("WARNING");
	}
	

	STATE.pQ -> q[STATE.pQ -> end] = *pCoor;

	++STATE.pQ -> end;
	STATE.pQ -> end %= QUEUE_SIZE;
}

void
queue_pull(void)
{
	++STATE.pQ -> start;
	STATE.pQ -> start %= QUEUE_SIZE;
}

bool
check_tile(uMap * pMap, tCoor * pCoor)
{
	if
	(
	 	pCoor -> y < 0 || pCoor -> x < 0 ||
		pCoor -> y > MAP_SIZE - 1 || pCoor -> x > MAP_SIZE - 1
	)
		return false;

	if(pMap -> map[pCoor -> y][pCoor -> x] != '.')
		return false;

	return true;
}

void
find_adj(tCoor adj[], tCoor * pCoor)
{
	adj[0].y = pCoor -> y - 1;
	adj[0].x = pCoor -> x;

	adj[1].y = pCoor -> y;
	adj[1].x = pCoor -> x + 1;

	adj[2].y = pCoor -> y + 1;
	adj[2].x = pCoor -> x;

	adj[3].y = pCoor -> y;
	adj[3].x = pCoor -> x - 1;
}

void
function(uMap * pMap, tCoor * pStart)
{
	static uMap d;
	memset(&d, -1, sizeof(uMap));

	static short * pDistCur;

	pDistCur = &d.dist_map[pStart -> y][pStart -> x];
	*pDistCur = 0;

	queue_push(pStart);

	int i = 0;

	while(i < 10)
	{
		static tCoor * pCurrent;

		pCurrent = &STATE.pQ -> q[STATE.pQ -> start];
		pDistCur = &d.dist_map[pCurrent -> y][pCurrent -> x];

		{
			static short * pDistAdj;
			static tCoor adj[4];

			//find adjacent to current
			find_adj(&adj[0], pCurrent);
			
			//for every adjacent if legal push to queue and update distance
			for(int j = 0; j < 4; ++j)
			{
				pDistAdj = &d.dist_map[adj[j].y][adj[j].x];
				if(check_tile(pMap, &adj[j]) && *pDistAdj == -1)
				{
					queue_push(&adj[j]);
					*pDistAdj = 1 + *pDistCur;
				}
			}
		}
		queue_pull();
		++i;
	}
	print_map(&d);
}

//function that takes two coordinates
//sets first coor as start
//adds first to queue
//CYCLE
//if second is in queue, return
//else do one step of fill
//END CYCLE
//returns shortest distance


int main(void)
{
	STATE.pQ = (tQueue *) calloc(1, sizeof(tQueue));
	STATE.pMap = (uMap *) calloc(1, sizeof(uMap));
	memset(STATE.pMap -> map, '.', sizeof(uMap));

	tCoor buf = {3, 6};
	function(STATE.pMap, &buf);
	return 0;
}
