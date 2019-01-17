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

unsigned int
count_differences(char *, char *);

void
find_similar_string(ID_List *, char *);

int main(void)
{
	ID_List * plist = make_list();
	parse_input(plist);
	for(int i = 0; i < plist->list_length; ++i)
	{
		count_letters(plist->list[i]);
	}
	printf("%d %d %d\n", ans[0], ans[1], ans[0] * ans[1]);
	//count_differences("tsdr", "asdf");
	for(int i = 0; i < plist->list_length; ++i)
	{
		find_similar_string(plist, plist->list[i]->id);
	}
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

unsigned int
count_differences(char * str1, char * str2)
{
	//puts(str1);
	//puts(str2);
	unsigned int dif = 0;
	while(*str1 && *str2)
	{
		if(*str1 != *str2)
			++dif;

		++str1;
		++str2;
	}
	//printf("They differ by %u characters.\n", dif);
	return dif;
}

void
find_similar_string(ID_List * plist, char * str)
{
	char * str2;
	uint8_t l = plist->list_length;
	for(int i = 0; i < l; ++i)
	{
		str2 = plist->list[i]->id;
		//printf("str2:%p l:%d\n", str2, l);
		if(count_differences(str, str2) == 1)
		{
			printf("%s\n is similar to\n%s\n", str, str2);
		}
	}
}
