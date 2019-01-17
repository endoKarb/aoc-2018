//Advent of Code 2018 Day 14
#include <stdio.h>
#include <string.h>

#define SIZE (1 << 25)
#define ITERATIONS (SIZE / 2)

typedef struct {
	unsigned long size;
	unsigned char lb[SIZE];
	unsigned long elves[2];
	unsigned char answer[6];
} tState;

tState STATE;

void
move_elves()
{
	STATE.elves[0] += (STATE.lb[STATE.elves[0]] + 1);
	STATE.elves[1] += (STATE.lb[STATE.elves[1]] + 1);

	STATE.elves[0] %= STATE.size;
	STATE.elves[1] %= STATE.size;
}

void
check_answer(void)
{
	static const size_t len = 6;
	static unsigned char * start;
	//last recipe;
	
	start = &STATE.lb[STATE.size - 1 - len];
	//printf("%u %u\n", *start, STATE.answer[0]);
	
	if(!memcmp(STATE.answer, start, len * sizeof(unsigned char)))
		printf("answer: %u\n", STATE.size - len - 1);
}

void
new_recipies(void)
{
	static unsigned char sum;
	static unsigned char new;

	sum = STATE.lb[STATE.elves[0]] + STATE.lb[STATE.elves[1]];

	if(sum > 9)
	{
		new = sum / 10;
		STATE.lb[STATE.size] = new;
		STATE.size += 1;
		check_answer();
	}

	new = sum % 10;
	STATE.lb[STATE.size] = new;
	STATE.size += 1;
	check_answer();

	move_elves();
}

void
print_lb(void)
{
	printf("elf0: %3u, elf1: %3u\n", STATE.elves[0], STATE.elves[1]);
	for(int i = 0; i < STATE.size; ++i)
	{
		printf("%u ", STATE.lb[i]);
	}
	putchar('\n');
}

int main(void)
{
	STATE.size = 2;
	STATE.lb[0] = 3;
	STATE.lb[1] = 7;
	STATE.elves[0] = 0;
	STATE.elves[1] = 1;
	
	STATE.answer[0] = 0;
	STATE.answer[1] = 8;
	STATE.answer[2] = 4;
	STATE.answer[3] = 6;
	STATE.answer[4] = 0;
	STATE.answer[5] = 1;

	for(int i = 0; i < ITERATIONS; ++i)
	{
		new_recipies();
		//print_lb();
	}

	return 0;
}
