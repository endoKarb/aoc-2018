//Advent of Code 2018 Day 11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERIAL 9424
#define GRID_SIZE 300

typedef struct {
	unsigned short x;
	unsigned short y;
} tCoor;

typedef struct {
	signed char grid[GRID_SIZE][GRID_SIZE];
} tGrid;

typedef struct {
	tCoor coor;
	unsigned short size;
	int power;

} tAnswer;

int
cell_fuel(tCoor Coor)
{
	static int fuel;
	static unsigned short rackID;

	rackID = Coor.x + 10;
	fuel = rackID * Coor.y;
	fuel += SERIAL;
	fuel *= rackID;
	fuel = (fuel % 1000) / 100;
	fuel -= 5;
	//printf("fuel: %d, rackID: %hu\n", fuel, rackID);

	return fuel;
}

void
every_cell(tGrid * pGrid)
{
	tCoor coor;
	for(int i = 0; i < GRID_SIZE; ++i)
	{
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			coor.y = i;
			coor.x = j;
			pGrid -> grid[i][j] = cell_fuel(coor);
			//printf("i%d j%d fuel%d\n", i, j, pGrid -> grid[i][j]);
		}
	}	

}

void
print_grid(tGrid * pGrid)
{
	for(int i = 0; i < GRID_SIZE; ++i)
	{
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			printf("%3d ", pGrid -> grid[i][j]);
		}
		putchar('\n');
	}
}

int
find_border_power(tGrid * pGrid, tCoor Coor, unsigned short size)
{
	int power = 0;
	//add column
	for(int i = 0; i < size; ++i)
	{
		power += pGrid -> grid[Coor.y + i][Coor.x + (size - 1)];
	}
	for(int i = 0; i < size - 1; ++i)
	{
		power += pGrid -> grid[Coor.y + size - 1][Coor.x + i];
	}
	//add row
	return power;
}

tAnswer
scan_every_size(tGrid * pGrid, tCoor Coor)
{
	//initialize answer as 0,0 size 1
	tAnswer answer;
	memset(&answer, 0, sizeof(tAnswer));
	answer.coor = Coor;
	answer.power = pGrid -> grid[Coor.y][Coor.x];
	answer.size = 1;

	//printf("x:%u, y:%u\n", Coor.x, Coor.y);
	int buf = answer.power;
	int i = 1;
	//printf("value:%d size:%u\n", buf, i);
	
	for(i = 2; i + Coor.y <= GRID_SIZE && i + Coor.x <= GRID_SIZE; ++i)
	{
		buf += find_border_power(pGrid, Coor, i);
		if(buf > answer.power)
		{
			answer.power = buf;
			answer.size = i;
		}
		//printf("value:%d size:%u\n", buf, i);
	}

	return answer;
}

tAnswer
best_area(tGrid * pGrid)
{
	tAnswer best_answer;
	memset(&best_answer, 1, sizeof(tAnswer));
	best_answer.power = pGrid -> grid[0][0];

	static tAnswer current_answer;

	for(int i = 0; i < GRID_SIZE; ++i)
	{
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			//using current_ans as buffer
			current_answer.coor.y = i;
			current_answer.coor.x = j;

			current_answer = scan_every_size(pGrid, current_answer.coor);
			if(current_answer.power > best_answer.power)
			{
				memcpy(&best_answer, &current_answer, sizeof(tAnswer));
			}


		}
	}
	return best_answer;
}


int main(void)
{
	tGrid * pGrid = (tGrid *) calloc(1, sizeof(tGrid));
	every_cell(pGrid);
	//print_grid(pGrid);

	tAnswer answer = best_area(pGrid);

	printf("The highest powered zone is at %hu,%hu with %d power at %hu size.\n", answer.coor.x, answer.coor.y, answer.power, answer.size);
	return 0;
}
