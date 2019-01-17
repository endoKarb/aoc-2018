//Advent of Code 2018 Day 9

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX_VALUE 7162600
#define PLAYERS 438

typedef struct marble{
	uintmax_t value;
	struct marble * right;
	struct marble * left;
} Marble;

typedef struct{
	uintmax_t next_value;
	uintmax_t scores[PLAYERS];
	unsigned int active_player;
	Marble marbles_pouch[MAX_VALUE + 1];
	Marble * current;
} State;

//GLOBALS
State STATE;

uintmax_t
highest_score()
{
	uintmax_t max = 0;
	for(int i = 0; i < PLAYERS; ++i)
	{
		if(STATE.scores[i] > max)
			max = STATE.scores[i];
	}
	return max;
}

void
print_circle()
{
	Marble * scan = STATE.current;
	do
	{
		printf("%-3" PRIuMAX " ", scan -> value);
		scan = scan -> right;
	}
	while(scan -> value != STATE.current -> value);
	putchar('\n');
}

Marble *
marble_zero()
{
	//create new Marble
	Marble * new = &STATE.marbles_pouch[STATE.next_value];
	//marble zero is to the left and the right of itself
	new -> right = new;
	new -> left = new;
	//set value to 0
	if(!STATE.next_value)
	{
		new -> value = STATE.next_value;
		++STATE.next_value;
	}
	else
		puts("Invalid value for marble 0!");
		
	return new;
}

//insert marble left of marble X
//marble x is x marbles right of current
void
insert_marble(unsigned int x)
{
	//create a marble with value = to next_value
	Marble * new = &STATE.marbles_pouch[STATE.next_value];
	new -> value = STATE.next_value;

	Marble * right = STATE.current;

	//shift x to the right
	while(x)
	{
		right = right -> right;
		--x;
	}

	Marble * left = right -> left;

	//set new marble left and right
	new -> right = right;
	new -> left = left;

	//place it in the circle
	right -> left = new;
	left -> right = new;

	//update current
	STATE.current = new;

}

//remove marble X marbles to the left of current
void
remove_marble(unsigned int x)
{
	Marble * target = STATE.current;
	
	//shift x to the left
	while(x)
	{
		target = target -> left;
		--x;
	}

	//update score
	STATE.scores[STATE.active_player] += STATE.next_value;
	STATE.scores[STATE.active_player] += target -> value;

	Marble * right = target -> right;
	Marble * left = target -> left;

	//remove target marble (strictly speaking not needed)
	//memset(&target -> right, 0, sizeof(Marble *) * 2);
	//printf("%p %p\n", target -> right, target -> left);
	target -> right = NULL;
	target -> left = NULL;
	/*
	*/

	//make right and left adjacent
	right -> left = left;
	left -> right = right;

	//update current
	STATE.current = right;
}

void
play_turn()
{
//	printf("Elf %u's turn.\n", STATE.active_player);
	switch(STATE.next_value % 23)
	{
	case 0:
		remove_marble(7);
		break;
	default:
		insert_marble(2);
		break;
	}
	
	//warn for size
	if(STATE.next_value == UINTMAX_MAX)
		puts("warning");
	//increment next_value
	++STATE.next_value;
	//next player
	++STATE.active_player;
	STATE.active_player %= PLAYERS;
}

int main(void)
{
	STATE.current = marble_zero();
	while(STATE.current -> value != MAX_VALUE)
	{
		play_turn();
//		print_circle();
	}
	printf("The winning elf scored %" PRIuMAX " points.\n", highest_score());
	return 0;
}
