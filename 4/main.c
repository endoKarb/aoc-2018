//Advent of Code 2018 Day 4
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STR_SIZE 128
#define LIST_SIZE 2048
#define GLIST_SIZE 4096

typedef struct {
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
} Time;

typedef enum {WAKE, SLEEP, SHIFT} Inst;

typedef struct {
	Time time;
	unsigned int ID;
	Inst instruction;
	char inst[STR_SIZE];
} Line;

typedef struct {
	Line * list[LIST_SIZE];
	unsigned int tot;
} Input;

typedef unsigned int Minutes[60];

typedef struct {
	unsigned int sleeptotal;
	Minutes sleepmin;
	bool slept;
} Guard;

typedef Guard Guardlist[GLIST_SIZE];


void
parse_input(Input *);
void
print_line(Line *);
void
sort_input(Input *);
int
cmp(const void * p1, const void * p2);
void
print_inst(Input *);
void
parse_inst(Input *);
void
compute_sleep(Input *, Guardlist);
unsigned int
find_sleepy(Guardlist glist);
void
track_minutes (Guard *, unsigned int, unsigned int);
unsigned int
find_sleepiest_minute (Guard *);
unsigned int
compute_answer2(Guardlist);

int
main(void)
{
	Guardlist * pglist = (Guardlist *) calloc(1, sizeof(Guardlist));
	Input * inp = (Input *) calloc(1, sizeof(Input));
	parse_input(inp);
	sort_input(inp);
	parse_inst(inp);
	//print_inst(inp);
	compute_sleep(inp, *pglist);
	unsigned int sleepy = find_sleepy(*pglist);
	unsigned int sleepiest_min = find_sleepiest_minute(&(*pglist)[sleepy]);

	printf("The answer to the first part is: %u.\n", sleepy * sleepiest_min);

	unsigned int answer2 = compute_answer2(*pglist);

	printf("The answer to the second part is: %u.\n", answer2); 

	return 0;
}

void
parse_input(Input * inp)
{
	Line * pbuf = (Line *) calloc(1, sizeof(Line));

	while(scanf("[%u-%u-%u %u:%u] %[^\n]\n",
		&pbuf->time.year, &pbuf->time.month, &pbuf->time.day,
		&pbuf->time.hour, &pbuf->time.minute,
		pbuf->inst) == 6)
	{
		inp->list[inp->tot] = pbuf;

		pbuf = (Line *) calloc(1, sizeof(Line));

		++inp->tot;
	}
	free(pbuf);
}

void
print_line(Line * pline)
{
	printf("%u-%.2u-%.2u %.2u:%.2u %s\n",
	pline->time.year, pline->time.month, pline->time.day,
	pline->time.hour, pline->time.minute,
	pline->inst);
}

void
sort_input(Input * pinp)
{
	Line ** base = pinp->list;
	unsigned int nmemb = pinp->tot;
	unsigned int size = sizeof(base);

	qsort(base, nmemb, size, cmp);
}

int
cmp(const void * p1, const void * p2)
{
	Line ** l1 = (Line **) p1;
	Line ** l2 = (Line **) p2;

	/*
	printf("%p %p\n", p1, l1);
	printf("size: %zu size:%zu\n", sizeof(p1), sizeof(l1));
	printf("%d\n", (*l1)->time.day);
	*/

	int res = (*l1)->time.month - (*l2)->time.month;
	if(res == 0)
		res = (*l1)->time.day - (*l2)->time.day;
	if(res == 0)
		res = (*l1)->time.hour - (*l2)->time.hour;
	if(res == 0)
		res = (*l1)->time.minute - (*l2)->time.minute;

	return res;
}

void
print_inst(Input * pinp)
{
	Line ** scan = pinp->list;
	while(*scan)
	{
		printf("Guard: %u, Inst: %u.\n", (*scan)->ID, (*scan)->instruction);
		++scan;
	}
}

void
parse_inst(Input * pinp)
{
	unsigned int watch;
	Line ** scan = pinp->list;

	while(*scan)
	{
		if((*scan)->inst[0] == 'G')
		{
			sscanf(scan[0]->inst, "Guard #%u", &watch);
			(*scan)->instruction = SHIFT;
		}
		else if((*scan)->inst[0] == 'w')
		{
			(*scan)->instruction = WAKE;
		}
		else if((*scan)->inst[0] == 'f')
		{
			(*scan)->instruction = SLEEP;
		}
		(*scan)->ID = watch;
		++scan;
	}

}

void
compute_sleep(Input * pinp, Guardlist glist)
{
	unsigned int sleep_start;

	Line ** scan = pinp->list;
	while(*scan)
	{
		if((*scan)->instruction == SLEEP)
		{
			sleep_start = (*scan)->time.minute;

			//printf("minute:%.2u ", sleep_start);
			//printf("%u sleeps\n", (*scan)->ID);
		}
		if((*scan)->instruction == WAKE)
		{
			Guard * b = &glist[(*scan)->ID];

			b->slept = true;
			b->sleeptotal += (*scan)->time.minute - sleep_start;
			//printf("minute:%.2u ", (*scan)->time.minute);
			//printf("%u wakes. he slept a total of %u minutes\n", (*scan)->ID, b->sleeptotal);
			track_minutes (b, sleep_start, (*scan)->time.minute);

		}
		++scan;
	}
}

unsigned int
find_sleepy(Guardlist glist)
{
	Guard * sleepy = &glist[0];

	for(int i = 0; i < GLIST_SIZE; ++i)
	{
		if(glist[i].slept && glist[i].sleeptotal > sleepy->sleeptotal)
		{
			sleepy = &glist[i]; 
		}
	}

//	printf("%p %u\n", sleepy, sleepy->sleeptotal);
	//printf("%u\n", sleepy - &glist[0]);

	return sleepy - &glist[0];
}

void
track_minutes (Guard * pgar, unsigned int m1, unsigned int m2)
{
	//printf("%u %u\n", m1, m2);	
	while(m1 < m2)
	{
		++pgar->sleepmin[m1];
		++m1;
	}
}

unsigned int
find_sleepiest_minute(Guard * pgar)
{
	unsigned int * s = &pgar->sleepmin[0];

	for(int i = 0; i < 60; ++i)
	{
		if(pgar->sleepmin[i] > *s)
		{ 
			s = &pgar->sleepmin[i];
		//	printf("%u %u\n", *s, s - &pgar->sleepmin[0]);
		}
		
	}

	return s - &pgar->sleepmin[0];
}

unsigned int
compute_answer2(Guardlist glist)
{
	Guard * sleepy = &glist[0];
	unsigned int m1 = find_sleepiest_minute(&glist[0]);
	unsigned int s1 = glist[0].sleepmin[m1];

	for(int i = 1; i < GLIST_SIZE; ++i)
	{
		unsigned int m2 = find_sleepiest_minute(&glist[i]);
		unsigned int s2 = glist[i].sleepmin[m2];

		
		if(s2 > s1)
		{
			printf("id: %u, m: %u, s:%u\n", i, m2, s2);
			sleepy = &glist[i];
			m1 = find_sleepiest_minute(&glist[i]);
			s1 = glist[i].sleepmin[m1];
		}
	}

	return (sleepy - &glist[0]) * m1;
}
