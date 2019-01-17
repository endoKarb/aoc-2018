//AoC 2018 Day 1
#include <stdio.h>
#include <stdint.h>
int main(void)
{
	int buf;
	uint16_t total = 0;
	int answer = 0;
	while(scanf("%d", &buf) == 1)
	{
		++total;
		answer += buf;
	}
	printf("From a total of %d instructions ", total);
	printf("the answer found was %d.\n", answer);
	return 0;
}
