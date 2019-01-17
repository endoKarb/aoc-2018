//Advent of Code 2018 Day 22
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TARGET_X 10
#define TARGET_Y 715
#define DEPTH 3339
//#define DEPTH 510
#define WIDTH 20
#define HIST_SIZE 1024

#define UP 4
#define DOWN 1
#define LEFT 3
#define RIGHT 2

#define NEITHER 0
#define TORCH 1
#define GEAR 2

#define ROCKY 0
#define WET 1
#define NARROW 2

typedef unsigned short uStep;
typedef signed short dCoor;

typedef struct{
	dCoor x;
	dCoor y;
} tCoor;

typedef struct{
	tCoor sPos;
	char tool;
} tStep;

typedef struct{
	tCoor sPos;
	dCoor x;
	dCoor y;
	uStep minutes;
	uStep steps;
	uStep shortest;
	tStep vHistory[HIST_SIZE];
	char tool;
} tStatus;

typedef struct{
	char type;
	size_t geoIndex;
	size_t erosion;
} tRegion;

typedef struct{
	tRegion vHead[DEPTH][WIDTH];
} tCave;

typedef struct{
	tCave sCave;
	size_t risk;
	unsigned short shortest;
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
			STATE.sCave.vHead[coor.y][coor.x - 1].erosion *
			STATE.sCave.vHead[coor.y - 1][coor.x].erosion;
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


bool
visited(const tStatus * pStatus, const dCoor y, const dCoor x)
{
	for(int i = 0; i < pStatus -> steps; ++i)
	{
		if(y == pStatus -> vHistory[i].sPos.y &&
			x == pStatus -> vHistory[i].sPos.x)
		{
			//print_history(pStatus);
			return true;
		}
	}
	return false;
}

void
switch_tool(tStatus * pStatus)
{
	//printf("y:%u x:%u ", pStatus -> y, pStatus -> x);
	//printf("terrain type:%u ", STATE.sCave.vHead[pStatus -> y][pStatus -> x].type);
	switch(STATE.sCave.vHead[pStatus -> y][pStatus -> x].type)
	{
		case ROCKY:
			if(pStatus -> tool == TORCH)
			{
				pStatus -> tool = GEAR;
			}
			else
			{
				pStatus -> tool = TORCH;
			}
			break;
		case WET:
			if(pStatus -> tool == GEAR)
			{
				pStatus -> tool = NEITHER;
			}
			else
			{
				pStatus -> tool = GEAR;
			}
			break;
		case NARROW:
			if(pStatus -> tool == NEITHER)
				pStatus -> tool = TORCH;
			else
				pStatus -> tool = NEITHER;
			break;
		default:
			puts("INVALID TOOL");
	}
}

void
print_history(const tStatus * pStatus)
{
	static char map[DEPTH][WIDTH];
	static dCoor y, x;
	static char ch;

	for(int i = 0; i < pStatus -> steps; ++i)
	{
		y = pStatus -> vHistory[i].sPos.y;
		x = pStatus -> vHistory[i].sPos.x;
		ch = pStatus -> vHistory[i].tool;
		map[y][x] = ch + '0';
	}

	for(int i = 0; i < 15; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			if(map[i][j] == 0)
			{
				switch(STATE.sCave.vHead[i][j].type)
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
			else
			{
				putchar(map[i][j]);
			}
		}
		putchar('\n');
	}
	memset (map, 0, sizeof(char) * DEPTH * WIDTH);
}

void
print_map(void)
{
	for(int i = 0; i < 20; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			switch(STATE.sCave.vHead[i][j].type)
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

bool
move(tStatus * pStatus, const char dir)
{
	//find tile
	dCoor x, y;

	x = pStatus -> x;
	y = pStatus -> y;

	switch(dir)
	{
		case UP:
			++y;
			break;
		case DOWN:
			--y;
			break;
		case LEFT:
			--x;
			break;
		case RIGHT:
			++x;
			break;
	}
	//check validity
	if
	(
		 y < 0 || x < 0 || y > DEPTH || x > WIDTH ||
		 pStatus -> steps > 1024 || visited(pStatus, y, x) ||
		 pStatus -> minutes >= STATE.shortest
	)
	{
		return false;
	}
	
	if(STATE.sCave.vHead[y][x].type == pStatus -> tool)
	{
		pStatus -> minutes += 7;
		switch_tool(pStatus);
	}
	//increase minute and steps
	pStatus -> minutes += 1;
	pStatus -> steps += 1;

	//switch if you have to
	//printf("type: %u, tool: %u\n", STATE.sCave.vHead[y][x].type, pStatus -> tool);
	
	//change pos
	pStatus -> y = y;
	pStatus -> x = x;

	return true;
}

uStep
sim(tStatus sStatus, const unsigned char dir, const unsigned short ID)
{

/*
	printf("pos: %hd %hd ID: %hu\n", sStatus.y, sStatus.x, ID);
	printf("steps: %hu, move: %u, minute: %u\n", sStatus.steps, dir, sStatus.minutes);
*/
	if(!move(&sStatus, dir))
	{
		return -1;
	}


	sStatus.vHistory[sStatus.steps].sPos.x = sStatus.x;
	sStatus.vHistory[sStatus.steps].sPos.y = sStatus.y;
	sStatus.vHistory[sStatus.steps].tool = sStatus.tool;


/*
	printf("pos: %hd %hd\n", sStatus.y, sStatus.x);
	printf("steps: %hu, move: %u\n", sStatus.steps, move);
	printf("SHORTEST %d\n", sStatus.shortest);
*/
	if(sStatus.y == TARGET_Y && sStatus.x == TARGET_X)
	{
		if(sStatus.tool != TORCH)
			sStatus.minutes += 7;
		if(sStatus.minutes < STATE.shortest)
		{
			printf("pos: %hd %hd ID: %hu\n", sStatus.y, sStatus.x, ID);
			printf("steps: %hu, move: %u, minute: %u\n", sStatus.steps, dir, sStatus.minutes);

			print_history(&sStatus);
			STATE.shortest = sStatus.minutes;
		}
		return sStatus.minutes;
	}

/*
	printf("pos: %hd %hd, tool: %u, type: %u\n",
		sStatus.y, sStatus.x, sStatus.tool,
		STATE.sCave.vHead[sStatus.y][sStatus.x].type);
*/
	uStep buf = -1;

	for(int i = 1; i <= 4; ++i)
	{
		buf = sim(sStatus, i, ID + 1);
		sStatus.shortest = (buf < sStatus.shortest) ? buf : sStatus.shortest;
	}

	return sStatus.shortest;
}

int main(void)
{
	tRegion * pScan;
	tCoor sBuf;
	for(int i = 0; i < DEPTH; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			pScan = &STATE.sCave.vHead[i][j];
			sBuf.y = i;
			sBuf.x = j;
			compute_geoIndex(pScan, sBuf);
			compute_erosion(pScan);
			compute_risk(pScan -> type, sBuf);
			//putchar('\n');
		}
	}

	print_map();

	tStatus * pStatus = (tStatus *) calloc(1, sizeof(tStatus));

	STATE.shortest = -1;
	pStatus -> shortest = -1;
	pStatus -> tool = 1;
	uStep shortest = sim(*pStatus, 0, 0);
	printf("SHORTEST PATH:%u\n", shortest);
/*
*/

	return 0;
}
