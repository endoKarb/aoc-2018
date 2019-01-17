//Advent of Code 2018 Day 7
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define JOBS 32
#define STEPS 32
#define TO_ALPH ('A' - 1)

typedef struct{
	bool reqs[STEPS];
	int reqs_left;
	int level;
} STEP;

typedef struct{
	STEP list[STEPS];
	unsigned int last_step;
} STEP_LIST;

void
scan_input(STEP_LIST * plist)
{
	char c1, c2, max;
	while(scanf("%*s %c %*s %*s %*s %*s %*s %c %*s %*s\n", &c1, &c2) == 2)
	{
		//to alph
		c1 = c1 - TO_ALPH;
		c2 = c2 - TO_ALPH;

		if(c1 > max)
			max = c1;
		if(c2 > max)
			max = c2;

		plist->list[c2].reqs[c1] = true;
		++plist->list[c2].reqs_left;
	}
	plist->last_step = max;
}

void
update_reqs(STEP_LIST * plist, unsigned int u)
{
	STEP * current;

	for(int i = 1; i <= plist->last_step; ++i)
	{
		current = &plist->list[i];

		if(current->reqs[u])
		{
			current->reqs[u] = false;
			--current->reqs_left;
		}
	}
}

int
find_next_job(STEP_LIST * plist)
{
	STEP * current;
	unsigned int next_step = 0;
	//find alphabetically first job with no requirements left
	for(int i = 1; i <= plist->last_step; ++i)
	{
		current = &plist->list[i];

		if(!current->reqs_left)
		{
			update_reqs(plist, i); //remove step found from all reqs
			--current->reqs_left;
			next_step = i;
			break;
		}
	}
	//update requirements
	//return
	return next_step;
}

char *
find_answer(STEP_LIST * plist)
{
	char * answer;
	answer = (char *) calloc(JOBS, sizeof(char));

	for(int i = 0; i < JOBS; ++i)
	{
		answer[i] = find_next_job(plist);
		//convert to ASCII letters
		if (answer[i] > 0)
			answer[i] += TO_ALPH;
	}

	return answer;
}

int main(void)
{
	char * answer;
	STEP_LIST * plist = (STEP_LIST *) calloc (1, sizeof(STEP_LIST));

	scan_input(plist);
	answer = find_answer(plist);

	printf("The steps should be completed in %s order.\n", answer);
	return 0;
}
