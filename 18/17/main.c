//Advent of Code 2018 Day 17
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAP_S (1 << 10)

typedef struct{
	unsigned int yMin;
	unsigned int yMax;
	unsigned int xMin;
	unsigned int xMax;
} tLimits;

typedef struct{
	unsigned int y;
	unsigned int x;
} tWell;

typedef struct{
	char vHead[MAP_S][MAP_S];
} tMap;

typedef struct{
	tMap sMap;
	tLimits sLim;
	tWell sWell;
} tState;

tState STATE;

void
build_wall(char dir, unsigned int a, unsigned int b1, unsigned int b2)
{
	switch(dir)
	{
		case 'y':
			for(int i = b1; i <= b2; ++i)
			{
				STATE.sMap.vHead[a][i] = '#'
			}
			break;
		case 'x'
			for(int i = b1; i <= b2; ++i)
			{
				STATE.sMap.vHead[i][a] = '#'
			}
			break;
	}
}

void
parse_input(void)
{
	memset(STATE.sMap.vHead, '.', sizeof(tMap));
	char ch;
	unsigned int a, b1, b2;
	while(scanf("%c=%u, %*c=%u..%u\n", &ch, &a, &b1, &b2) == 4)
	{
		build_wall(ch, a, b1, b2);
	}
}

void
print_map(void)
{
	for(int y = STATE.sLim.yMin; y < STATE.sLim.yMax; ++y)
	{
		for(int x = STATE.sLim.xMin; x < STATE.sLim.xMax; ++x)
		{
			putchar(STATE.sMap.vHead[y][x]);
		}
		putchar('\n');
	}
}

bool
check_closed(unsigned int y, unsigned int x)
{
	static unsigned int left, right;
	static char bottom, current;
	static bool leftbound, rightbound;

	leftbound = rightbound = false;
	left = right = x;

	current = STATE.sMap.vHead[y][x];
	bottom = STATE.sMap.vHead[y + 1][x];

	while(bottom == '#' || bottom == '~')
	{
		if(current == '#')
		{
			leftbound = true;
			break;
		}
		--left;
		current = STATE.sMap.vHead[y][left];
		bottom = STATE.sMap.vHead[y + 1][left];
		printf("y:%u, left:%u\n", y, left);
	}

	if(leftbound)
	{
		current = STATE.sMap.vHead[y][x];
		bottom = STATE.sMap.vHead[y + 1][x];
		while(bottom == '#' || bottom == '~')
		{
			if(current == '#')
			{
				rightbound = true;
				break;
			}
			++right;
			current = STATE.sMap.vHead[y][right];
			bottom = STATE.sMap.vHead[y + 1][right];
		}
	}
	else
	{
		return false;
	}

	if(leftbound && rightbound)
	{
		for(int i = left + 1; i < right; ++i)
		{
			STATE.sMap.vHead[y][i] = '~';
		}
		return true;
	}

	return false;
}

void
drop(unsigned int y, unsigned int x, unsigned char ID)
{
	static char under;
	static char left;
	static char right;

	bool end = false;

	while(y < STATE.sLim.yMax && !end)
	{
		printf("ID:%u\n", ID);
		print_map();
		under = STATE.sMap.vHead[y + 1][x];
		switch(under)
		{
			case '.':
				++y;
				STATE.sMap.vHead[y][x] = '|';
				break;
			case '~':
			case '#':
				if(check_closed(y, x))
				{
					--y;
					break;
				}
				left = STATE.sMap.vHead[y][x - 1];
				right = STATE.sMap.vHead[y][x + 1];
				if(left == '.' && right == '.')
				{
					STATE.sMap.vHead[y][x - 1] = '|';
					drop(y, x - 1, ID + 1);
				}
				left = STATE.sMap.vHead[y][x - 1];
				right = STATE.sMap.vHead[y][x + 1];
				/*
				printf("%u %u\n", y, x);
				printf("%c %c\n",
					STATE.sMap.vHead[y][x-1],
					STATE.sMap.vHead[y][x+1]);
				*/
				if(left == '.')
				{
					--x;
					STATE.sMap.vHead[y][x] = '|';
				}
				else if(right == '.')
				{
					++x;
					STATE.sMap.vHead[y][x] = '|';
				}
				else
				{
					end = true;
				}
				break;
		}
	}
}

int main(void)
{
	parse_input();
	drop(STATE.sWell.y, STATE.sWell.x, 0);
	return 0;
}
