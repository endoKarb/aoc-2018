//Advent of Code 2018 Day 5

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#define ALPH_S 26
#define POLY_S 51200
#define CASE ('A' - 'a')

unsigned int
react(char *);
unsigned int
react_all(char *);
bool
check_reaction(const char, const char);
char *
strip_reagent(const char *, const char);
unsigned int
strip_and_check(const char *, const char);
unsigned int
shortest_poly(const char *);
int
smaller(const void *, const void *);

int
main(void)
{
	char * polymer = (char *) calloc(POLY_S, sizeof(char));
	scanf("%s", polymer);

	unsigned int answer1 = react_all(polymer);
	printf("The first answer is %d.\n", answer1);

	unsigned int answer2 = shortest_poly(polymer);
	printf("The second answer is %d.\n", answer2);

	return 0;
}

unsigned int
react(char * poly)
{
	char * scan = poly;
	char * dest = poly;

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

	return count;
}

bool
check_reaction(const char c1, const char c2)
{
	return (c1 + CASE) == c2 || (c2 + CASE) == c1;
}

unsigned int
react_all(char * poly)
{
	while(react(poly))
		;
	return strlen(poly);
}

char *
strip_reagent(const char * src, const char ch)
{
	static char * buf;
	static bool first = true;

	if(first)
	{
		buf = (char *) calloc(POLY_S, sizeof(char));
		first = false;
	}

	unsigned int count = 0;
	const char * scan = src;
	char * dest = buf;

	while(*scan)
	{
		if(tolower(*scan) == tolower(ch))
		{
			++scan;
			continue;
		}

		*dest = *scan;

		++scan;
		++dest;
	}

	*dest = '\0';

	return buf;
}

unsigned int
strip_and_check(const char * poly, const char ch)
{
	char * p = strip_reagent(poly, ch);
	return react_all(p);
}

unsigned int
shortest_poly(const char * poly)
{
	unsigned int alphabet[ALPH_S];

	for(int i = 0; i < ALPH_S; ++i)
	{
		alphabet[i] = strip_and_check(poly, 'a' + i);
	}

	qsort(alphabet, ALPH_S, sizeof(alphabet[0]), smaller);

	return alphabet[0];
}

int
smaller(const void * p1, const void * p2)
{

	unsigned int * u1 = (unsigned int *) p1;
	unsigned int * u2 = (unsigned int *) p2;

	return *u1 - *u2;
}
