//Advent of Code 2018 Day 5

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#define POLY_S 51200
#define ALPH_S 26
#define CASE ('A' - 'a')

char *
scan_polymer();
unsigned int
react(char **, char **);
bool
check_reaction(char, char);
unsigned int
strip_reagent(char **, char **, char);
unsigned int
find_best_removal(char **, char **, char **);

int main(void)
{
	unsigned int answer[2];
	char * polymer = scan_polymer();

	char * polycopy = (char *) calloc(POLY_S, sizeof(char));
	char * buffer = (char *) calloc(POLY_S, sizeof(char));

	strcpy(polycopy, polymer);

	while(react(&polycopy, &buffer))
		;

	answer[0] = strlen(polycopy);

	answer[1] = find_best_removal(&polymer, &polycopy, &buffer);
	
	printf("The answer to the first part is %d.\n", answer[0]);
	printf("The answer to the second part is %d.\n", answer[1]);
	return 0;
}

char *
scan_polymer()
{
	char * p = (char *) calloc(POLY_S, sizeof(char));
	char * scan;
	for(scan = p; isalpha(*scan = getchar()) ;++scan)
	{
	}
	char a, b;
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

unsigned int
strip_reagent(char ** str, char ** buf, char ch)
{
	unsigned int count = 0;
	char * scan = *str;
	char * dest = *buf;
	while(*scan)
	{
		if(tolower(*scan) == tolower(ch))
		{
			++count;
			++scan;
			continue;
		}
		*dest = *scan;
		++dest;
		++scan;
	}
	*dest = '\0';

	printf("found %d %c\n", count, ch);

	strcpy(*str, *buf);

	return count;
}

unsigned int
find_best_removal(char ** polymer, char ** polycopy, char ** buffer)
{
	unsigned int alphabet[ALPH_S];
	char ch = 'a';

	while(ch <= 'z')
	{
		strcpy(*polycopy, *polymer);
		strip_reagent(polycopy, buffer, ch);
		//puts(*polycopy);
		while(react(polycopy, buffer))
			;
		//puts(*polycopy);
		alphabet[ch - 'a'] = strlen(*polycopy);
		printf("removing %c gives a length of %d.\n\n", ch, alphabet[ch-'a']);
		++ch;
	}

	unsigned int r = alphabet[0];
	for(int i = 0; i < ALPH_S; ++i)
	{
		if(alphabet[i] < r)
		{
			r = alphabet[i];
		}
	}

	return r;
}
