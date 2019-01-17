//AoC 2018 Day 7
#include <stdio.h>
#include <stdbool.h>

#define ALPH_SIZE 32
#define BUFF_SIZE 256
#define ELVES_SIZE 5
#define OFFSET 60
#define FIRST_LETTER 5
#define SECOND_LETTER 36

typedef struct {
	bool reqs[ALPH_SIZE];
	bool taken; 
	int reqs_left; /* -1 == step done */
} STEP;

typedef struct {
	int job;
	int secs_left;
} ELF;

typedef struct {
	STEP steps[ALPH_SIZE]; 
	unsigned int steps_size;
	unsigned int steps_done;
	char buffer[BUFF_SIZE];
	char answer[ALPH_SIZE];
	ELF elves[ELVES_SIZE];
	unsigned int time;
	bool all_done;
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


		STATE.steps[c2].reqs[c1] = true;
		++STATE.steps[c2].reqs_left;
		
		puts(STATE.buffer);
	}
	STATE.steps_size = max + 1;
}

int
pick_job()
{
	for(int i = 1; i < STATE.steps_size; ++i)
	{
		if(!STATE.steps[i].reqs_left && !STATE.steps[i].taken)
		{
			STATE.steps[i].taken = true;
			//printf("Job %c has been started.\n", i + '@');
			return i;
		}
	}
	return 0;
}

void
job_done(const unsigned int job)
{
	if(!job)
	{
		printf("empty job\n");
		return;
	}


	STATE.steps[job].reqs_left = -1;

	for(int i = 1; i < STATE.steps_size; ++i)
	{
		if(STATE.steps[i].reqs[job])
		{
			STATE.steps[i].reqs[job] == false;
			--STATE.steps[i].reqs_left;
		}
		
	}

	++STATE.steps_done;
	//printf("Job %c is done!.\n", job + '@');
}


void
work(ELF * elf)
{
	for(int i = 0; i < ELVES_SIZE; ++i)
	{
		if(elf[i].job)
		{
			printf("Elf %u: %d seconds left for job %c\n", i, elf[i].secs_left, elf[i].job + '@');
			--elf[i].secs_left;
		}
	}
}

void
check_finished(ELF * elf)
{
	for(int i = 0; i < ELVES_SIZE; ++i)
	{
		if(elf[i].job && !elf[i].secs_left)
		{
			printf("Elf %d: Job %c done!\n", i, elf[i].job + '@');
			job_done(elf[i].job);
			elf[i].job = false;
		}
	}
}

void
new_job(ELF * elf)
{
	unsigned int jobs_found = ELVES_SIZE;
	for(int i = 0; i < ELVES_SIZE; ++i)
	{
		if(!elf[i].job)
		{
			elf[i].job = pick_job();
			if(elf[i].job)
			{
				printf("Elf %d: Job %c started!\n", i, elf[i].job + '@');
				elf[i].secs_left = elf[i].job + OFFSET;
			}
			else
			{
				--jobs_found;
				printf("Elf %d: No jobs available!\n", i);
			}
		}
	}

	if(!jobs_found)
		STATE.all_done = true;
}

int
main(void)
{
	parse_input();
	for(int i = 0; !STATE.all_done; ++i)
	{
		STATE.time = i;
		printf("\n### Second %u ###\n", STATE.time);
		//check completed jobs
		check_finished(STATE.elves);
		//assign new jobs
		new_job(STATE.elves);
		//work
		work(STATE.elves);
	}
	puts("All jobs completed! The sliegh is ready!!!.");
	printf("It only took %d seconds, using %d elves!\n", STATE.time, ELVES_SIZE);

	return 0;
}
