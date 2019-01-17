//Advent of Code 2018 Day 14
#include <stdio.h>

#define SIZE 1024000

typedef struct {
	unsigned long size;
	unsigned char lb[SIZE];
	unsigned long elves[2];

	unsigned long limit;
	unsigned long answer[10];
	unsigned long answer_size;
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
new_recipies()
{
	static unsigned char sum;
	static unsigned char new;

	sum = STATE.lb[STATE.elves[0]] + STATE.lb[STATE.elves[1]];

	if(sum > 9)
	{
		new = sum / 10;
		STATE.lb[STATE.size] = new;

		//save if over limit
		if(STATE.size >= STATE.limit && STATE.answer_size < 10)
		{
			STATE.answer[STATE.answer_size] = new;
			++STATE.answer_size;
		}

		STATE.size += 1;
	}

	new = sum % 10;
	STATE.lb[STATE.size] = new;

	if(STATE.size >= STATE.limit && STATE.answer_size < 10)
	{
		STATE.answer[STATE.answer_size] = new;
		++STATE.answer_size;
	}

	STATE.size += 1;

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

void
print_answer(void)
{
	for(int i = 0; i < STATE.answer_size; ++i)
	{
		printf("%u ", STATE.answer[i]);
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
	STATE.limit = 84601;

	print_lb();
	for(int i = 0; i < STATE.limit + 20; ++i)
	{
		new_recipies();
		//print_lb();
	}

	print_answer();

	return 0;
}
