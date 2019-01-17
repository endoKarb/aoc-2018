//Advent of Code 2018 Day 10
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define nPOINTS (1 << 9)
#define MAP_SIZE (1 << 6)

typedef struct
{
	long x;
	long y;
} tCoor;

typedef struct
{
	signed char x;
	signed char y;
} tSpeed;

typedef struct
{
	tCoor topleft;
	tCoor bottomright;
	unsigned long long height;
	unsigned long long width;
	unsigned long long size;
} tArea;

typedef struct
{
	tCoor position;
	tSpeed velocity;
} tPoint;

typedef struct
{
	tPoint points[nPOINTS];
	unsigned int length;
	tArea area;
} tPList;

typedef struct
{
	bool map[MAP_SIZE][MAP_SIZE];
} tMap;

tPList PLIST;
tPList LAST_PLIST;
unsigned int TIME;

void
print_area(tArea * parea)
{
	printf("tlx: %d, tly: %d\n", parea -> topleft.x, parea -> topleft.y);
	printf("brx: %d, bry: %d\n", parea -> bottomright.x, parea -> bottomright.y);
	printf("Total area: %llu\n", parea -> size);
}

tArea
find_area()
{
	tArea area;

	area.topleft.x = PLIST.points[0].position.x;
	area.topleft.y = PLIST.points[0].position.y;

	area.bottomright.x = PLIST.points[0].position.x;
	area.bottomright.y = PLIST.points[0].position.y;

	for(size_t i = 0; i < PLIST.length; ++i)
	{
		if(PLIST.points[i].position.x < area.topleft.x)
			area.topleft.x = PLIST.points[i].position.x;
		if(PLIST.points[i].position.y < area.topleft.y)
			area.topleft.y = PLIST.points[i].position.y;
		if(PLIST.points[i].position.x > area.bottomright.x)
			area.bottomright.x = PLIST.points[i].position.x;
		if(PLIST.points[i].position.y > area.bottomright.y)
			area.bottomright.y = PLIST.points[i].position.y;
	}
	
	area.width = area.bottomright.x - area.topleft.x; 
	area.height = area.bottomright.y - area.topleft.y; 

	area.size = area.height * area.width;
	//printf("%llu", area.size - 1);

	//print_area(&area);
	return area;
}

void
parse_input()
{
	int min_x = 0;
	int min_y = 0;

	tPoint * scan = PLIST.points;
	char ch;
	while((ch = getchar()) != EOF)
	{
		if(ch != '<')
			continue;
		else
		{
			scanf("%ld, %ld",&scan->position.x,&scan->position.y);
			if(scan->position.x < min_x)
				min_x = scan -> position.x;
			if(scan->position.y < min_y)
				min_y = scan -> position.y;
		}
		while((ch = getchar()) != '<')
			;
		{
			scanf("%hhd, %hhd",&scan->velocity.x,&scan->velocity.y);
			/*
			printf("%ld %ld ", scan->start_pos.x, scan->start_pos.y);
			printf("%hhd %hhd\n", scan->velocity.x, scan->velocity.y);
			*/
			++PLIST.length;
			++scan;
		}
	}
	PLIST.area = find_area();
	print_area(&PLIST.area);
	//printf("%d %d\n", min_x, min_y);
}


void
clear_map(tMap * pmap)
{
	memset(pmap, 0, sizeof(tMap));
}

void
print_map(tMap * pmap)
{
	for(size_t i = 0; i <= LAST_PLIST.area.height; ++i)
	{
		for(size_t j = 0; j <= LAST_PLIST.area.width; ++j)
		{
			if(pmap -> map[i][j])
				putchar('#');
			else
				putchar('.');
		}
		putchar('\n');
	}
}

void
update_map(tMap * pmap)
{
	clear_map(pmap);

	int x_off = LAST_PLIST.area.topleft.x;
	int y_off = LAST_PLIST.area.topleft.y;

	int x;
	int y;
	

	for(size_t i = 0; i < LAST_PLIST.length; ++i)
	{
		x = LAST_PLIST.points[i].position.x - x_off;
		y = LAST_PLIST.points[i].position.y - y_off;
		//printf("x: %d, y: %d\n", x, y);

		pmap -> map[y][x] = true;
	}

}


void
print_points()
{
	puts("################################");
	int x, y;
	for(size_t i = 0; i < PLIST.length; ++i)
	{
		x = PLIST.points[i].position.x;
		y = PLIST.points[i].position.y;
		printf("x: %-8d, y: %-8d. ", x, y);
	}
	putchar('\n');
}

void
tick()
{
	LAST_PLIST = PLIST;
	tPoint * scan = PLIST.points;
	for(size_t i = 0; i < PLIST.length; ++i)
	{
		scan -> position.x += scan -> velocity.x;
		scan -> position.y += scan -> velocity.y;
	/*	
		printf("%d %d ", scan->position.x, scan->position.y);
		printf("%d %d\n", scan->velocity.x, scan->velocity.y);
	*/
		
		++scan;
	}	
	PLIST.area = find_area();
	++TIME;
}

int main(void)
{

	parse_input();

	tMap * pmap = (tMap *) calloc(1, sizeof(tMap));


	unsigned long size = PLIST.area.size;
	while(PLIST.area.size <= size)
	{
		size = PLIST.area.size;
		//print_area(&PLIST.area);
		tick();
	} 
	update_map(pmap);
	printf("Second: %u\n", TIME);
	print_area(&PLIST.area);
	print_map(pmap);
	
	return 0;
}
