//AoC 2018 Day 1
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#define SIZE 1000

typedef struct instrlist{
	int list[SIZE];
	uint16_t total;
	int answer[2];
} Instructions;

typedef struct counter{
	bool positive[1000000];
	bool negative[1000000];
	bool duplicate;
} Counter;

void init_inst(Instructions *);
void init_count(Counter *);
void parse_inst(Instructions *);
void compute_ans1(Instructions *);
void compute_ans2(Instructions *, Counter *);
bool search_dupe(Instructions *, Counter *);

int main(void)
{
	Counter count;
	Instructions inst;

	init_inst(&inst);
	init_count(&count);

	parse_inst(&inst);
	compute_ans1(&inst);
	compute_ans2(&inst, &count);

	printf("The answer to the first half is %d\n", inst.answer[0]);
	printf("The answer to the second half is %d\n", inst.answer[1]);

	return 0;
}

void init_inst(Instructions * inst)
{
	memset(inst, 0, sizeof(Instructions));
}
void init_count(Counter * count)
{
	memset(count, 0, sizeof(Counter));
}
void parse_inst(Instructions * inst)
{

	while(scanf("%d", &inst->list[inst->total]) == 1)
	{
		++inst->total;
	}

}
void compute_ans1(Instructions * inst)
{
	for(int i = 0; i < inst->total; ++i)
	{
		inst->answer[0] += inst->list[i];
	}
}
void compute_ans2(Instructions * inst, Counter * count)
{
	while(!count->duplicate)
	{
		count->duplicate = search_dupe(inst, count);
	}
}
bool search_dupe(Instructions * inst, Counter * count)
{
	return true;
}
