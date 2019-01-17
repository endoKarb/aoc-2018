//Advent of Code 2018 Day 12
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define POTS 512
#define OFFSET 10
#define GENERATIONS 50000000000

typedef struct{
	int sum;
} tAnswer;

typedef struct{
	bool pots[POTS];
} tPots;

typedef bool tCode[5];

typedef struct{
	tCode code;
} tNote;

typedef struct{
	tNote list[16];
	unsigned int size;
} tNoteList;

typedef struct{
	tAnswer answer;
	tPots current;
	tPots previous;
	tNoteList notes;
	unsigned int growth[256];
} tState;


void
parse_state(tState * pState)
{
	char ch;

	//eat characters
	while(ch != '.' && ch != '#')
		ch = getchar();

	//scan state
	for(int i = OFFSET; ch != '\n' && i < POTS; ++i)
	{
		if(ch == '#')
			pState -> current.pots[i] = true;
		ch = getchar();
	}

}

void
parse_notes(tNoteList * pNoteList)
{
	char ch;

	for(int i = 0; (ch = getchar()) != EOF; ++i)
	{
		++pNoteList -> size;

		//eat characters
		while(ch != '.' && ch != '#')
			ch = getchar();
			
		//copy code
		for(int j = 0; ch == '.' || ch == '#' || j > 5; ++j)
		{
			if(ch == '#')
				pNoteList -> list[i].code[j] = true;
			if(ch == '.')
				pNoteList -> list[i].code[j] = false;

			ch = getchar();
		}

		//eat characaters
		while((ch = getchar()) != '.' && ch != '#')
			;

		//if the note doesn't make the plant survive, discard it.
		if(ch == '.')
		{
			--pNoteList -> size;
			--i;
		}
	}
}

void
parse_input(tState * pState)
{
	parse_state(pState);
	parse_notes(&pState -> notes);
}

int
compute_answer(tPots * pPots)
{
	int sum = 0;
	for(int i = 0; i < POTS; ++i)
	{
		if(pPots -> pots[i])
		{
			sum += i - OFFSET;
			//printf("sum: %d\n", sum);
		}
	}
	return sum;
}

bool
find_matching_note(const tNoteList * pNoteList, const bool * pCurrent)
{
	//if the pattern matches the plant surives, else it dies
	static const tNote * scan;

	for(int i = 0; i < pNoteList -> size; ++i)
	{
		scan = &pNoteList -> list[i];
		if(!memcmp(scan -> code, pCurrent, sizeof(tCode)))
			return true;
	}
	return false;
}

void
tick_generation(tState * pState)
{
	//memcpy current to previous
	memcpy(pState -> previous.pots, pState -> current.pots, sizeof(tPots));
	//memset current
	memset(pState -> current.pots, 0, sizeof(tPots));

	static bool * scan;
	//stop 4 spaces before end
	for(int i = 0; i < POTS - 4; ++i)
	{
		scan = &pState -> previous.pots[i];
		pState -> current.pots[i + 2] = find_matching_note(&pState -> notes, scan);
	}
}

void
print_pots(tPots * pPots)
{
	for (int i = 0; i < POTS; ++i)
	{
		if(pPots -> pots[i])
			putchar('#');
		else
			putchar('.');
	}
	putchar('\n');
}

int main(void)
{
	tState * pState = (tState *) calloc(1, sizeof(tState));
	parse_input(pState);


	long long current;
	int previous;
	unsigned int * scan = &pState -> growth[0]; 
	bool stable = false;
	for(int i = 1; i <= GENERATIONS; ++i)
	{
		if(i >= 4 && *(scan - 3) == *(scan - 2) && *(scan - 2) == *(scan- 1))
		{
			stable = true;
		}

		if(stable)
		{
			current += (*(scan - 1)) * (GENERATIONS - i);
			break;
		}
		else
		{
			tick_generation(pState);
			current = compute_answer(&pState -> current);
			previous = compute_answer(&pState -> previous);
			*scan = current - previous;
			++scan;
			//printf("generation:%u, current: %lld, previous:%d, growth:%d\n", i, current, previous, pState -> growth[i]);
		}


		//print_pots(&pState -> current);
	}
	printf("After %llu generations, there are %lld plants\n", (unsigned long long)GENERATIONS, current);
	return 0;
}
