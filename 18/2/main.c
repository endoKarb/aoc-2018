//AoC 2018 Day 2.
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE_ID 64
#define SIZE_LIST 256
#define SIZE_ALPH 26

unsigned int ans[2] = {0,0};

typedef struct {
	char
	id[SIZE_ID];

	uint8_t
	occ[SIZE_ALPH];

	bool twice;
	bool thrice;
} ID;

typedef struct {
	ID *
	list[SIZE_LIST];

	uint8_t
	list_length;
} ID_List;

ID_List *
make_list();

ID *
make_ID();

void
count_letters(ID *);

void
parse_input(ID_List *);

void
appears_twice(ID *);

int main(void)
{
	ID_List * plist = make_list();
	parse_input(plist);
	for(int i = 0; i < plist->list_length; ++i)
	{
		count_letters(plist->list[i]);
	}
	printf("%d %d %d\n", ans[0], ans[1], ans[0] * ans[1]);
	return 0;
}

ID_List *
make_list()
{
	ID_List * p = (ID_List *) malloc(sizeof(ID_List));
	memset(p, 0, sizeof(ID_List));
	return p;
}

void
parse_input(ID_List * plist)
{
	char buf[64];
	while(scanf("%s", &buf) == 1)
	{
		plist->list[plist->list_length] = make_ID();
		strcpy(plist->list[plist->list_length]->id, buf);
		//puts(plist->list[plist->list_length]->id);
		++plist->list_length;
	}
}

ID *
make_ID()
{
	ID * p = (ID *) malloc(sizeof(ID));
	memset(p, 0, sizeof(ID));
	return p;
}

void
count_letters(ID * pid)
{
	char * scan = pid->id;
	while(*scan)
	{
		++pid->occ[*scan - 'a'];
		putchar(*scan);
		++scan;	
	}
	putchar('\n');
	appears_twice(pid);
}

void
appears_twice(ID * pid)
{
	for(int i = 0; i < SIZE_ALPH; ++i)
	{
		if(pid->occ[i] == 2)
			pid->twice = true;
		else if(pid->occ[i] == 3)
			pid->thrice = true;
	}
	if(pid->twice)
	{
		++ans[0];
		puts("twice!");
	}
	if(pid->thrice)
	{
		++ans[1];
		puts("thrice!");
	}
}
