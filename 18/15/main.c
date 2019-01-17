//Advent of Code 2018 Day 15
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAP_SIZE (1 << 6)
#define STARTING_HEALTH 200
#define MAX_UNITS (1 << 10)
#define MAX_WALLS (1 << 10)
#define MAX_QUEUE (1 << 6)

#define STAY 0
#define UP 1
#define LEFT 2
#define RIGHT 3
#define DOWN 4

typedef struct{
	short x;
	short y;
} tCoor;

typedef struct{
	unsigned char start;
	unsigned char end;
	unsigned char size;
	tCoor vHead[MAX_QUEUE];
} tQueue;

typedef struct{
	unsigned int round;
	int elves;
	int goblins;
} tScore;

typedef struct{
	unsigned char type;
	short health;
	tCoor coor;
} tUnit;

typedef struct{
	tUnit * pUnit;
} tMapTile;

typedef struct{
	unsigned int size;
	tUnit * pHead[MAX_UNITS];
} tUnitList;

typedef struct{
	tUnitList lUnit;
	tUnitList lWall;
} tObjects;

typedef struct{
	tMapTile vHead[MAP_SIZE][MAP_SIZE];
} tMap;

typedef struct{
	tCoor coor;
	unsigned char dir;
} tPath;

typedef struct{
	tScore sScore;
	tObjects sObj;
	tMap sMap;
} tState;


tState STATE;

tMapTile *
find_tile(tCoor coor)
{
	return &STATE.sMap.vHead[coor.y][coor.x];
}

void
add_unit(char type, tCoor coor)
{
	static tUnit vUnit[MAX_UNITS]; 
	static tUnitList * plUnit;
	static unsigned short size;

	switch(type)
	{
		case '#':
			plUnit = &STATE.sObj.lWall;
			break;
		default:
			plUnit = &STATE.sObj.lUnit;
			break;
	}

	tUnit ** ppUnit = &plUnit -> pHead[plUnit -> size];

	//record unit data
	vUnit[size].type = type;
	vUnit[size].coor = coor;
	vUnit[size].health = STARTING_HEALTH;

	//update unit list
	*ppUnit = &vUnit[size];
	
	//update map
	STATE.sMap.vHead[coor.y][coor.x].pUnit = *ppUnit;

	//increse size of list
	++size;
	++plUnit -> size;

	//print changes
	//printf("Added y%ux%u type:%c\n", coor.y, coor.x, type);
	//printf("Total units: %u\n", size);
}

void
compute_score(void)
{
	static tUnitList * plUnit = &STATE.sObj.lUnit;
	static tUnit * pUnit;

	STATE.sScore.elves = 0;
	STATE.sScore.goblins = 0;

	for(int i = 0; i < plUnit -> size; ++i)
	{
		pUnit = plUnit -> pHead[i]; 
		if(pUnit -> health > 0)
		{
			switch(pUnit -> type)
			{
				case 'G':
					STATE.sScore.goblins += pUnit -> health;
					break;
				case 'E':
					STATE.sScore.elves += pUnit -> health;
					break;
				case '#':
					break;
				default:
					puts("INVALID TYPE FOR SCORE");
					break;
			}
		}

	}	
}


void
parse_input(void)
{
	int ch;
	tCoor coor = {0, 0};

	//parse every character
	while((ch = getchar()) != EOF)
	{
		switch(ch)
		{
		//if '\n', increment move to next row, reset x
			case '\n':
				coor.x = -1;
				++coor.y;
				break;
		//for every E or G create a unit and increase score
			case 'E':
				add_unit(ch, coor);
				break;
			case 'G':
				add_unit(ch, coor);
				break;
		//for every # add wall
			case '#':
				add_unit(ch, coor);
				break;
		//for '.'?
			case '.':
				break;
			default:
				puts("INVALID CHARACTER");
				break;
		}
		//move to next column
		++coor.x;
	}
	
	//compute starting score
	compute_score();
}

int
earliest_coor(const void * v1, const void * v2)
{
	static const tUnit * u1;
	static const tUnit * u2;
	static signed char dis;

	u1 = *((tUnit **) v1);
	u2 = *((tUnit **) v2);

/*
	printf("type:%c y:%u, x:%u\n",
		pU1 -> type, pU1 -> coor.y, pU1 -> coor.x);
	printf("type:%c y:%u, x:%u\n",
		pU2 -> type, pU2 -> coor.y, pU2 -> coor.x);
*/

	dis = u1 -> coor.y - u2 -> coor.y;
	//printf("dis: %d\n", dis);

	if(dis)
		return dis;

	dis = u1 -> coor.x - u2 -> coor.x;
	//printf("dis: %d\n", dis);

	return dis;
}

void
sort_units(void)
{
	tUnitList * plUnit = &STATE.sObj.lUnit;

	qsort(
	plUnit -> pHead,
	plUnit -> size,
	sizeof(plUnit -> pHead[0]),
	earliest_coor
	);
}

void
prepare_map(char vChMap[][MAP_SIZE], char enemy)
{
	tUnit * pUnit;
	for(int i = 0; i < MAP_SIZE; ++i)
	{
		for(int j = 0; j < MAP_SIZE; ++j)
		{
			pUnit = STATE.sMap.vHead[i][j].pUnit;

			if(!pUnit || pUnit -> health <= 0)
				vChMap[i][j] = '.';
			else if(pUnit -> type == enemy)
				vChMap[i][j] = 'X';
			else
				vChMap[i][j] = '#';

			//putchar(vChMap[i][j]);
		}
		//putchar('\n');
	}
}

void
print_ChMap(char vChMap[][MAP_SIZE])
{
	for(int i = 0; i < MAP_SIZE; ++i)
	{
		for(int j = 0; j < MAP_SIZE; ++j)
		{
			putchar(vChMap[i][j]);
		}
		putchar('\n');
	}
}

void
empty_queue(tQueue * pQ)
{
	memset(pQ, 0, sizeof(tQueue));
}
void
queue_push(tQueue * pQ, tCoor pCoor)
{
	if(pQ -> size > MAX_QUEUE)
	{
		puts("QUEUE TOO LONG");
		return;
	}

	//append
	pQ -> vHead[pQ -> end] = pCoor;

	//printf("Added y:%u, x:%u\n", pCoor.y, pCoor.x);

	++pQ -> size;
	++pQ -> end;
	pQ -> end %= MAX_QUEUE;

}
tCoor
queue_pull(tQueue * pQ)
{
	tCoor sBuf;
	
	if(!pQ -> size)
	{
		puts("NO ELEMENTS IN QUEUE");
		return sBuf;
	}

	sBuf = pQ -> vHead[pQ -> start];

	--pQ -> size;
	++pQ -> start;
	pQ -> start %= MAX_QUEUE;
	
	//printf("Pulled y:%u, x:%u\n", sBuf.y, sBuf.x);

	return sBuf;
}


char
fill(char vChMap[][MAP_SIZE], tQueue * pQ)
{
	static tCoor vAdj[4];
	static tCoor sBuf;

	if(pQ -> size == 0)
	{
		//print_ChMap(vChMap);
		return STAY + '0';
	}
	
	tCoor vQueue[MAX_QUEUE];
	int qSize = 0;

	int dir;
	while(pQ -> size)
	{
		sBuf = queue_pull(pQ);
		dir = vChMap[sBuf.y][sBuf.x];

		vAdj[UP] = vAdj[LEFT] = vAdj[RIGHT] = vAdj[DOWN] = sBuf;

		--vAdj[UP].y;
		--vAdj[LEFT].x;
		++vAdj[RIGHT].x;
		++vAdj[DOWN].y;

		for(int i = 0; i <= DOWN; ++i)
		{
			char target;
			//printf("y:%d x:%d\n", vAdj[i].y, vAdj[i].x);
			if
			(
				vAdj[i].x >= 0 && vAdj[i].x < MAP_SIZE &&
				vAdj[i].y >= 0 && vAdj[i].y < MAP_SIZE
			)
			{
				target = vChMap[vAdj[i].y][vAdj[i].x];
				switch(target)
				{
					case '.':
						vChMap[vAdj[i].y][vAdj[i].x] = dir;
						vQueue[qSize] = vAdj[i];
						++qSize;
						break;
					case 'X':
						//print_ChMap(vChMap);
						return dir;
						break;
					default:
						break;
				}
			}
		}
		//print_ChMap(vChMap);
	}
	for(int i = 0; i < qSize; ++i)
	{
		queue_push(pQ, vQueue[i]);
	}
	dir = fill(vChMap, pQ);
	return dir;
}

tCoor
select_dest(char vChMap [][MAP_SIZE], tUnit * pUnit)
{
	//static tUnitList * plUnit = &STATE.sObj.lUnit;
	//static tUnit * pScan;
	static tCoor sCur;
	static tCoor vAdj[5];
	static tQueue sQ;

	sCur = pUnit -> coor;

	vAdj[STAY] = sCur;
	vAdj[UP] = sCur;
	--vAdj[UP].y;
	vAdj[LEFT] = sCur;
	--vAdj[LEFT].x;
	vAdj[RIGHT] = sCur;
	++vAdj[RIGHT].x;
	vAdj[DOWN] = sCur;
	++vAdj[DOWN].y;

	char * target;
	if(vAdj[UP].y >= 0)
	{
		target = &vChMap[vAdj[UP].y][vAdj[UP].x];
		switch(*target)
		{
			case '.':
				*target = UP + '0';
				queue_push(&sQ, vAdj[UP]);
				break;
			case 'X':
				return vAdj[UP]; 
			default:
				break;
		}
	}
	if(vAdj[LEFT].y >= 0)
	{
		target = &vChMap[vAdj[LEFT].y][vAdj[LEFT].x];
		switch(*target)
		{
			case '.':
				*target = LEFT + '0';
				queue_push(&sQ, vAdj[LEFT]);
				break;
			case 'X':
				return vAdj[LEFT]; 
			default:
				break;
		}
	}
	if(vAdj[RIGHT].y < MAP_SIZE)
	{
		target = &vChMap[vAdj[RIGHT].y][vAdj[RIGHT].x];
		switch(*target)
		{
			case '.':
				*target = RIGHT + '0';
				queue_push(&sQ, vAdj[RIGHT]);
				break;
			case 'X':
				return vAdj[RIGHT]; 
			default:
				break;
		}
	}
	if(vAdj[DOWN].y < MAP_SIZE)
	{
		target = & vChMap[vAdj[DOWN].y][vAdj[DOWN].x];
		switch(*target)
		{
			case '.':
				*target = DOWN + '0';
				queue_push(&sQ, vAdj[DOWN]);
				break;
			case 'X':
				return vAdj[DOWN]; 
			default:
				break;
		}
	}

	
	//if not found goblin yet
	//for every element in queue, write his char in all empty adjacent
	//if you find a goblin return early with direction
	//if you don't return null
	
	char dir = fill(vChMap, &sQ);
	printf("dir %d ", dir - '0');
	empty_queue(&sQ);

	return vAdj[dir - '0'];
}

bool
attack(char vChMap[][MAP_SIZE], tUnit * pUnit)
{
	static tCoor vAdj[5];
	static tUnit * pTarget;
	static tUnit * pBuf;

	pTarget = NULL;

	vAdj[STAY] = pUnit -> coor;
	vAdj[UP] = vAdj[STAY];
	--vAdj[UP].y;
	vAdj[LEFT] = vAdj[STAY];
	--vAdj[LEFT].x;
	vAdj[RIGHT] = vAdj[STAY];
	++vAdj[RIGHT].x;
	vAdj[DOWN] = vAdj[STAY];
	++vAdj[DOWN].y;

	for(int i = UP; i <= DOWN; ++i)
	{
		if(vChMap[vAdj[i].y][vAdj[i].x] == 'X')
		{
			pBuf = find_tile(vAdj[i]) -> pUnit;
			if(!pTarget || pBuf -> health < pTarget -> health)
			{
				pTarget = pBuf;
			}
		}
	}

	//print_ChMap(vChMap);
	if(pTarget)
	{
		printf("ATTACK!\n");
		pTarget -> health -= 3;
		return true;
	}
	else
		return false;
}

void
proc_unit(tUnit * pUnit)
{
	static tMapTile * pMapTile;
	static char vChMap[MAP_SIZE][MAP_SIZE];
	//static tUnit * pTarget;

	printf("%u %u\n", pUnit -> coor.y, pUnit -> coor.x);
	//printf("%u\n", find_tile(pUnit -> coor) -> coor.y);
	char enemy;
	if(pUnit -> type == 'G')
		enemy = 'E';
	else
		enemy = 'G';
	prepare_map(vChMap, enemy);
	print_ChMap(vChMap);
	
	//try to attack adjacent
	//if fail
	if(!attack(vChMap, pUnit))
	{
		//find a destination tile
		tCoor sDest = select_dest(vChMap, pUnit);
		pMapTile = find_tile(sDest);

		//move there
		puts("move");

		find_tile(pUnit -> coor) -> pUnit = NULL;
		pMapTile -> pUnit = pUnit;

		pUnit -> coor = sDest;

		//try to attack again
		attack(vChMap, pUnit);
	}

	//TEST END CONDITION BE CAREFUL
	//pUnit -> health -= 50;
}

void
tick(void)
{
	static tUnitList * plUnit = &STATE.sObj.lUnit;
	static tUnit * pUnit;

	//for every unit: proc it
	for(int i = 0; i < plUnit -> size; ++i)
	{
		pUnit = plUnit -> pHead[i];
		//printf("p: %p, type: %c, y:%u, x:%u\n" , pUnit, pUnit -> type, pUnit -> coor.y, pUnit -> coor.x);
		if(pUnit -> health > 0)
		{
			proc_unit(pUnit);
		}
	}	
	//update score
	compute_score();
	//sort unit list
	sort_units();
}

int main(void)
{
	unsigned int rounds = 0;
	//parse_input
	parse_input();
	//do rounds

	while(STATE.sScore.elves > 0 && STATE.sScore.goblins > 0)
	{
		tick();
		++rounds;
	}
	printf("Combat ends after %u rounds.\n", rounds);
	printf("Elves:%d, Goblins:%d\n", STATE.sScore.elves, STATE.sScore.goblins);
/*
	//print results
*/
	return 0;
}
