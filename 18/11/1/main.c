//Advent of Code 2018 Day 11
#include <stdio.h>
#include <stdlib.h>

#define SERIAL 9424
#define GRID_SIZE 300

typedef struct {
	unsigned short x;
	unsigned short y;
} tCoor;

typedef struct {
	signed char grid[GRID_SIZE][GRID_SIZE];
} tGrid;

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

signed char
area_power(tGrid * pGrid, tCoor Coor)
{
	//printf("y%d x%d fuel:%4d\n", Coor.y, Coor.x);
	signed char fuel = 0;
	for(int i = Coor.y; i < Coor.y + 3; ++i)
	{
		for(int j = Coor.x; j < Coor.x + 3; ++j)
		{
			fuel += pGrid -> grid[i][j];
		}
	}
	return fuel;
}

tCoor
best_area(tGrid * pGrid)
{
	tCoor best = {0, 0};
	signed char best_value = -100;

	static tCoor scan;
	static signed char current_value;

	for(int i = 0; i < GRID_SIZE - 2; ++i)
	{
		for(int j = 0; j < GRID_SIZE - 2; ++j)
		{
			scan.y = i;
			scan.x = j;
			//printf("y%d x%d\n", scan.y, scan.x);
			current_value = area_power(pGrid, scan);
			if(current_value > best_value)
			{
				printf("%hhd %hhd\n", current_value, best_value);
				best_value = current_value;
				best = scan;
				printf("y:%hu x:%hu\n\n\n", best.y, best.x);
			}


		}
	}
	return best;
}


int main(void)
{
	tGrid * pGrid = (tGrid *) calloc(1, sizeof(tGrid));
	every_cell(pGrid);

	tCoor answer = best_area(pGrid);

	printf("The answer is %hu,%hu.\n", answer.x, answer.y);
	return 0;
}
