//Advent of Code 2018 Day 11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERIAL 18
#define GRID_SIZE 9424

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
	int fuel;

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

int FUEL = 0;

int
area_power(tGrid * pGrid, tCoor Coor, unsigned short size)
{
	int fuel = 0;
	for(int i = Coor.y; i < Coor.y + size; ++i)
	{
		for(int j = Coor.x; j < Coor.x + size; ++j)
		{
			fuel += pGrid -> grid[i][j];
			if (fuel > FUEL)
			{
				FUEL = fuel;
				printf("highest fuel %d at coord %u,%u, with size %u\n", FUEL, Coor.x, Coor.y, size);
			}
		}
	}
	//printf("y%d x%d fuel:%4d size:%u\n", Coor.y, Coor.x, fuel, size);
	return fuel;
}

tAnswer
scan_every_size(tGrid * pGrid, tCoor Coor)
{
	tAnswer answer;
	memset(&answer, 0, sizeof(tAnswer));
	answer.fuel = -120;

	unsigned short largest_coor;
	//find largest coordinate
	if(Coor.x > Coor.y)
		largest_coor = Coor.x;
	else
		largest_coor = Coor.y;

	//find largest area size from current coord;
	unsigned short largest_area_size = GRID_SIZE - largest_coor;

	//call area power for every legal area size
	for(int i = 1; i <= largest_area_size; ++i)
	{
		int buf = area_power(pGrid, Coor, i);
		if(buf > answer.fuel)
		{
			answer.fuel = buf;
			answer.coor = Coor;
			answer.size = i;
		}
	}

	return answer;
}

tAnswer
best_area(tGrid * pGrid)
{
	tAnswer best_answer;
	memset(&best_answer, 1, sizeof(tAnswer));
	best_answer.fuel = -120;

	static tAnswer current_answer;

	for(int i = 0; i < GRID_SIZE; ++i)
	{
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			current_answer.coor.y = i;
			current_answer.coor.x = j;

			current_answer = scan_every_size(pGrid, current_answer.coor);
			if(current_answer.fuel > best_answer.fuel)
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

	tAnswer answer = best_area(pGrid);

	printf("The answer is %hu,%hu with %d fuel at %hu size.\n", answer.coor.x, answer.coor.y, answer.fuel, answer.size);
	return 0;
}
