//Advent of Code 2018 Day 22
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define TARGET_X 10
#define TARGET_Y 715
#define DEPTH 3339
//#define DEPTH 510
#define WIDTH 50

#define MAX_QUEUE 512

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define NEITHER 0
#define TORCH 1
#define GEAR 2

#define ROCKY 0
#define WET 1
#define NARROW 2

typedef unsigned char uType;
typedef unsigned short uSize;
typedef signed short dCoor;

typedef struct{
	dCoor x;
	dCoor y;
} tCoor;

typedef struct{
	uType type;
	size_t geoIndex;
	size_t erosion;
} tRegion;

typedef struct{
	uSize vVisited[3];
} tTile;

typedef union{
	tRegion vRegion[DEPTH][WIDTH];
	tTile vTile[DEPTH][WIDTH];
} uCave;

typedef struct{
	uSize uTime;
	tCoor sCoor;
	uType uTool;
} tItem;

typedef struct{
	uSize uSize;
	tItem vItem[MAX_QUEUE];
} tQueue;

typedef struct{
	uSize uTime;
	uCave sCave;
	size_t risk;
} tState;

tState STATE;

void
compute_erosion(tRegion * pScan)
{
	pScan -> erosion = (pScan -> geoIndex + DEPTH) % 20183;
	pScan -> type = pScan -> erosion % 3;
	//printf("erosion:%zu type:%u\n", pScan -> erosion, pScan -> eType);
}

void
compute_geoIndex(tRegion * pScan, tCoor coor)
{
	if(coor.y == 0 && coor.x == 0)
	{
		pScan -> geoIndex = 0;
	}
	else if(coor.y == TARGET_Y && coor.x == TARGET_X)
	{
		pScan -> geoIndex = 0;
	}
	else if(coor.y == 0)
	{
		pScan -> geoIndex = coor.x * 16807;
	}
	else if(coor.x == 0)
	{
		pScan -> geoIndex = coor.y * 48271;
	}
	else
	{
		pScan -> geoIndex =
			STATE.sCave.vRegion[coor.y][coor.x - 1].erosion *
			STATE.sCave.vRegion[coor.y - 1][coor.x].erosion;
	}
	//printf("x:%zu y:%zu\ngeoIndex:%zu\n", coor.x, coor.y, pScan -> geoIndex);
}

void
compute_risk(const char risk, const tCoor coor)
{
	if(coor.y <= TARGET_Y && coor.x <= TARGET_X)
	{
		STATE.risk += risk;
	}
	//printf("current risk:%zu\n", STATE.risk);
}

void
print_map(void)
{
	for(int i = 0; i < 10; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			switch(STATE.sCave.vRegion[i][j].type)
			{
				case ROCKY:
					putchar('.');
					break;
				case WET:
					putchar('=');
					break;
				case NARROW:
					putchar('|');
					break;
			}
		}
		putchar('\n');
	}
}

uType
tile_type(tCoor sCoor)
{
	return STATE.sCave.vRegion[sCoor.y][sCoor.x].type;
}

void
qpush(tQueue * pQueue, tItem * pItem)
{
	assert(pQueue -> uSize != MAX_QUEUE);
	pQueue -> vItem[pQueue -> uSize] = *pItem;
	//printf("SIZE: %u\n", pQueue -> uSize);
	++pQueue -> uSize;
}

void
mark_visited(uCave * pMap, tItem * pItem)
{
	pMap -> vTile[pItem -> sCoor.y][pItem -> sCoor.x].vVisited[pItem -> uTool] = pItem -> uTime;
}

bool
is_inside(tCoor sCoor)
{
	if(sCoor.y >= 0 && sCoor.x >= 0 && sCoor.y < DEPTH && sCoor.x < WIDTH)
		return true;
	//printf("INVALID y:%hd, x:%hd\n", sCoor.y, sCoor.x);
	return false;
}

void
get_adj(tCoor vAdj[4], tCoor sCoor)
{
	for(int i = 0; i < 4; ++i)
	{
		vAdj[i] = sCoor;
	}

	--vAdj[UP].y;
	--vAdj[LEFT].x;
	++vAdj[RIGHT].x;
	++vAdj[DOWN].y;

	/*
	printf("y:%d, x:%d\n", sCoor.y, sCoor.x);
	printf("UP y:%d, x:%d\n", vAdj[UP].y, vAdj[UP].x);
	printf("LEFT y:%d, x:%d\n", vAdj[LEFT].y, vAdj[LEFT].x);
	printf("RIGHT y:%d, x:%d\n", vAdj[RIGHT].y, vAdj[RIGHT].x);
	printf("DOWN y:%d, x:%d\n", vAdj[DOWN].y, vAdj[DOWN].x);
	 */
}

bool
need_switch(tCoor sDest, uType Tool)
{
	if(tile_type(sDest) == Tool)
		return true;
	else
		return false;
}

uType
change_tool(tItem * pItem)
{
	uType tool = pItem -> uTool;
	uType type = tile_type(pItem -> sCoor);

	assert(type != tool);
	if(type != TORCH && tool != TORCH)
		return TORCH;
	if(type != NEITHER && tool != NEITHER)
		return NEITHER;
	if(type != GEAR && tool != GEAR)
		return GEAR;
	else
		return -1;
}


bool
is_visited(uCave * pMap, tItem * pItem)
{
	uType Tool = pItem -> uTool;

	if(pMap -> vTile[pItem -> sCoor.y][pItem -> sCoor.x].vVisited[Tool] <= pItem -> uTime)
		return true;
	else
		return false;
}

void
do_item(uCave * pMap, tItem * pItem, tQueue * pQueue)
{
/*
*/
	if(pItem -> sCoor.y == 715 && pItem -> sCoor.x == 10)
	{
		puts("Current Item");
		printf("y%u x%u tool:%u type:%u\n", pItem -> sCoor.y, pItem -> sCoor.x, pItem -> uTool, tile_type(pItem -> sCoor));
	}

	tItem sBufItem;
	tCoor vAdj[4];
	get_adj(vAdj, pItem -> sCoor);

	for(int i = 0; i < 4; ++i)
	{ 
		if(!is_inside(vAdj[i]))
			continue;

		//puts("BREAK");
		sBufItem.uTool = pItem -> uTool;
		sBufItem.uTime = pItem -> uTime + 1;
		sBufItem.sCoor = vAdj[i];

		if(need_switch(sBufItem.sCoor, sBufItem.uTool))
		{
			sBufItem.uTime += 7;
			sBufItem.uTool = change_tool(pItem);
		}
		
		if(!is_visited(pMap, &sBufItem))
		{
			qpush(pQueue, &sBufItem);
			mark_visited(pMap, &sBufItem);
		}
	}

}

bool
function(uCave * pMap)
{
	//printf("Current time: %u\n", STATE.uTime);
	static tQueue sQueue = {1, { {0, {0,0}, TORCH} } };

	tQueue sBuffer = {0};

	tItem * pScan;
	for(int i = 0; i < sQueue.uSize; ++i)
	{
		pScan = &sQueue.vItem[i];
		switch(pScan -> uTime == STATE.uTime)
		{
		case true:
			do_item(pMap, pScan, &sBuffer);
			//printf("Time: %u\n", pScan -> uTime);
			//printf("y:%u x:%u\n", pScan -> sCoor.y, pScan -> sCoor.x);
			break;
		case false:
			qpush(&sBuffer, pScan);
			//printf("size: %u\n", sBuffer.uSize);
			break;
		}
	}

	STATE.uTime += 1;

	memcpy(&sQueue, &sBuffer, sizeof(tQueue));

	return sQueue.uSize;
}

int main(void)
{
	tRegion * pScan;
	tCoor sBuf;
	for(int i = 0; i < DEPTH; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			pScan = &STATE.sCave.vRegion[i][j];
			sBuf.y = i;
			sBuf.x = j;
			compute_geoIndex(pScan, sBuf);
			compute_erosion(pScan);
			compute_risk(pScan -> type, sBuf);
			//putchar('\n');
		}
	}

	print_map();
	uCave sMap;

	memset(&sMap.vTile, -1, sizeof(sMap));

	while(function(&sMap))
		;

	return 0;
}
