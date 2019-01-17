//AoC 2018 Day 7
#include <stdio.h>
#include <stdbool.h>

#define ALPH_SIZE 32
#define BUFF_SIZE 256
#define FIRST_LETTER 5
#define SECOND_LETTER 36

typedef struct {
	bool req[ALPH_SIZE];
	unsigned int reqs_left;
} STEP;

typedef struct {
	STEP steps[ALPH_SIZE]; 
	unsigned int steps_size;
	char buffer[BUFF_SIZE];
	char answer[ALPH_SIZE];
} GLOBAL_STATE;

GLOBAL_STATE STATE;

void
parse_input()
{
	unsigned int c1, c2;
	unsigned int max = 0;

	while(scanf("%255[^\n]\n", &STATE.buffer) == 1)
	{
		c1 = STATE.buffer[FIRST_LETTER] - '@';
		c2 = STATE.buffer[SECOND_LETTER] - '@';


		if(c1 > max)
			max = c1;
		if(c2 > max)
			max = c2;


		STATE.steps[c2].req[c1] = true;
		++STATE.steps[c2].reqs_left;
		
		puts(STATE.buffer);
		/*
		for(int i = 0; i < ALPH_SIZE; ++i)
		{
			printf("%d ", STATE.steps[c2].req[i]);
		}
		printf("reqs left: %d ", STATE.steps[c2].reqs_left);
		puts("");
		*/
	}
	STATE.steps_size = max + 1;
}

void
remove_req(const unsigned int step)
{
	for(int i = 1; i < STATE.steps_size; ++i)
	{
		if(STATE.steps[i].req[step])
		{
			STATE.steps[i].req[step] = false;
			--STATE.steps[i].reqs_left;
		}
	}
}

int
execute_step()
{
	for(int i = 1; i < STATE.steps_size; ++i)
	{
		if(STATE.steps[i].reqs_left == 0)
		{
			remove_req(i);
			--STATE.steps[i].reqs_left;
			printf("%c", i + '@');
			return i + '@';
		}
	}
	putchar('\n');
	printf("No steps left to execute.\n");
	return -1;
}


int
main(void)
{
	parse_input();

	for(int i = 0; execute_step() != -1; ++i)
		;

	return 0;
}
