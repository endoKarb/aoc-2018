//Advent of Code 2018 Day 18
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAP_SIZE (1 << 8)

int
max(int a, int b)
{
	if(a > b)
		return a;
	else
		return b;
}

bool
check_trees(const char map[][MAP_SIZE], const int y, const int x)
{
	int count = 0;
	for(int row = max(y - 1, 0); row < y + 2 ;++row)
	{
		for(int col = max(x - 1, 0); col < x + 2 ;++col)
		{
			if(map[row][col] == '#')
				++count;
		}
	}
	if (count >= 3)
		return true;
	else
		return false;
}

bool
check_lumberyard(const char map[][MAP_SIZE], const int y, const int x)
{
	int lumberyards = 0;
	bool trees = false;
	for(int row = max(y - 1, 0); row < y + 2 ;++row)
	{
		for(int col = max(x - 1, 0); col < x + 2 ;++col)
		{
			if(map[row][col] == '#')
				++lumberyards;
			else if(map[row][col] == '|')
				trees = true;
		}
	}
	if (lumberyards >= 2 && trees)
		return false;
	else
		return true;
}

bool
check_open(const char map[][MAP_SIZE], const int y, const int x)
{
	int count = 0;
	for(int row = max(y - 1, 0); row < y + 2 ;++row)
	{
		for(int col = max(x - 1, 0); col < x + 2 ;++col)
		{
			if(map[row][col] == '|')
				++count;
		}
	}
	if (count >= 3)
		return true;
	else
		return false;
}

char
check_adj(const char map[][MAP_SIZE], const int row, const int col)
{
	char new;
	bool check = false;

	switch(map[row][col])
	{
		case '.':
			check = check_open(map, row, col);
			new = '|';
			break;
		case '|':
			check = check_trees(map, row, col);
			new = '#';
			break;
		case '#':
			check = check_lumberyard(map, row, col);
			new = '.';
			break;
		default:
			puts("INVALID TILE");
	}
	if(check)
		return new;
	else
		return map[row][col];
}

void
tick(char map1[][MAP_SIZE], char map2[][MAP_SIZE])
{
	int row, col;
	for(row = 0; map1[row][0] != 0; ++row)
	{
		for(col = 0; map1[row][col] != 0; ++col)
		{
			map2[row][col] = check_adj(map1, row, col);
		}
	}
	memcpy(map1, map2, sizeof(char) * MAP_SIZE * MAP_SIZE);
}

void
print_map(char map[][MAP_SIZE])
{
	int i, j = 0;
	for(i = 0; map[i][j] != 0; ++i)
	{
		for(int j = 0; j < MAP_SIZE; ++j)
		{
			putchar(map[i][j]);
		}
		putchar('\n');
	}
}

void
parse_input(char map[][MAP_SIZE])
{
	int ch;
	int row = 0, col = 0;
	while((ch = getchar()) != EOF)
	{
		switch(ch)
		{
			case '\n':
				col = -1;
				++row;
				break;
			case '.':
			case '#':
			case '|':
				map[row][col] = ch;
				break;
			default:
				puts("INVALID INPUT");
		}
		++col;
	}
}

int
compute_answer(char map[][MAP_SIZE])
{
	int wood = 0, lumber = 0;
	for(int i = 0; map[i][0] != 0; ++i)
	{
		for(int j = 0; map[i][j] != 0; ++j)
		{
			if(map[i][j] == '|')
				++wood;
			else if(map[i][j] == '#')
				++lumber;
		}
	}
	return wood * lumber;
}

int main(void)
{
	int answer, i;
	char map1[MAP_SIZE][MAP_SIZE] = {0};
	char map2[MAP_SIZE][MAP_SIZE] = {0};

	parse_input(map1);
	//for(int i = 0; answer != 181383; ++i)
	for(i = 0; i < 1504; ++i)
	{
		tick(map1, map2);
	}
	answer = compute_answer(map2);
	printf("The resource value at minute %d is %d\n", i, answer);
	//print_map(map2);


	return 0;
}
