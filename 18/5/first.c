//Advent of Code 2018 Day 5

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#define POLY_S 51200
#define CASE ('A' - 'a')

char *
scan_polymer();
unsigned int
react(char **, char **);
void
react_all(char **);
bool
check_reaction(char, char);

int
main(void)
{
	char * polymer = scan_polymer();

	react_all(&polymer);

	unsigned int answer = strlen(polymer);

	printf("The answer is %d.\n", answer);

	return 0;
}

char *
scan_polymer()
{
	char * p = (char *) calloc(POLY_S, sizeof(char));
	char * scan;
	for(scan = p; isalpha(*scan = getchar()) ;++scan)
		;
	*scan = '\0';
	return p;
}

unsigned int
react(char ** str, char ** buf)
{
	memset(*buf, POLY_S, sizeof(char));
	char * scan = *str;
	char * dest = *buf;

	unsigned int count = 0;

	while(*scan)
	{
		if(check_reaction(*scan, *(scan + 1)))
		{
			++count;
			scan += 2;
			continue;
		}

		*dest = *scan;
		++scan;
		++dest;
	}

	*dest = '\0';

	strcpy(*str, *buf);

	return count;
}

bool
check_reaction(char c1, char c2)
{
	return (c1 + CASE) == c2 || (c2 + CASE) == c1;
}

void
react_all(char ** polymer)
{
	char * buffer = (char *) calloc(POLY_S, sizeof(char));
	while(react(polymer, &buffer))
		;
}
