//Advent of Code 2018 Day 23
#include <stdio.h>
#include <stdlib.h>

#define MAX_BOT 1024

typedef signed long dCoor;
typedef unsigned long uSize;

typedef struct{
	dCoor x;
	dCoor y;
	dCoor z;
} tPoint;

typedef struct{
	tPoint sPoint;
	uSize uRange;
} tNanoBot;

typedef struct{
	uSize uSize;
	tNanoBot vHead[MAX_BOT];
} tBotList;

void
parse_input(tBotList * pBotList)
{
	tNanoBot sBuf;

	while(scanf("pos=<%ld,%ld,%ld>, r=%lu\n",
				&sBuf.sPoint.x,
				&sBuf.sPoint.y,
				&sBuf.sPoint.z,
				&sBuf.uRange)
			== 4)
	{
		pBotList -> vHead[pBotList -> uSize] = sBuf;
		++pBotList -> uSize;
	}
	
	printf("Parsed %lu nanoBots.\n", pBotList -> uSize);
}

tNanoBot *
best_nanobot(tBotList * pBotList)
{
	tNanoBot * pScan = &pBotList -> vHead[0];
	tNanoBot * pBest = pScan;

	for(int i = 1; i < pBotList -> uSize; ++i)
	{
		pScan = &pBotList -> vHead[i];
		if(pScan -> uRange > pBest -> uRange)
			pBest = pScan;
	}
	return pBest;
}

uSize
man_dist(tNanoBot * pFirst, tNanoBot * pSecond)
{
	uSize uDist = 0;

	uDist += abs(pFirst->sPoint.x - pSecond->sPoint.x);
	uDist += abs(pFirst->sPoint.y - pSecond->sPoint.y);
	uDist += abs(pFirst->sPoint.z - pSecond->sPoint.z);

	//printf("%lu\n", uDist);

	return uDist;
}

uSize
count_inrange(tBotList * pBotList, tNanoBot * pNanoBot)
{
	uSize uInRange = 0;
	tNanoBot * pScan;
	for(int i = 0; i < pBotList -> uSize; ++i)
	{
		pScan = &pBotList -> vHead[i];
		if(man_dist(pScan, pNanoBot) <= pNanoBot -> uRange)
		{
			++uInRange;
		}
	}
	printf("In Range: %lu\n", uInRange);
	return uInRange;
}

uSize
find_nearest(tBotList * pBotList, tNanoBot * pNanoBot)
{
	uSize uShortest = -1;
	uSize uBuf;
	tNanoBot * pScan;
	for(int i = 0; i < pBotList -> uSize; ++i)
	{
		pScan = &pBotList -> vHead[i];
		uBuf = man_dist(pScan, pNanoBot);
		if(uBuf < uShortest)
			uShortest = uBuf;
	}
	printf("Smallest distance: %lu\n", uShortest);
	return uShortest;
}

uSize
count_adj(tBotList * pBotList, tNanoBot * pNanoBot)
{
	uSize uInRange = 0;
	tNanoBot * pScan;
	for(int i = 0; i < pBotList -> uSize; ++i)
	{
		pScan = &pBotList -> vHead[i];
		if(man_dist(pScan, pNanoBot) <= pScan -> uRange)
		{
			++uInRange;
		}
	}
	printf("Adjacent: %lu\n", uInRange);
	return uInRange;
}

int main(void)
{
	tBotList * pBotList = (tBotList *) calloc(1, sizeof(tBotList));
	parse_input(pBotList);
	//count_inrange(pBotList, best_nanobot(pBotList));

	tNanoBot sBuf;
	tNanoBot sMax;
	uSize uMult = 10000000;
	uSize uAdd = -10000000;
	uSize uAdj = 0;
	uSize uMax = 0;
	for(int i = 0; i < 10000; ++i)
	{
/*
		printf("%lu %lu %lu\n",
				sBuf.sPoint.x, sBuf.sPoint.y, sBuf.sPoint.z);
*/
		sBuf.sPoint.x = (i * uMult) + uAdd;
		uAdj = count_adj(pBotList, &sBuf);
		if(uAdj > uMax)
		{
			uMax = uAdj;
			sMax = sBuf;
		}

		sBuf.sPoint.y = (i * uMult) + uAdd;
		uAdj = count_adj(pBotList, &sBuf);
		if(uAdj > uMax)
		{
			uMax = uAdj;
			sMax = sBuf;
		}

		sBuf.sPoint.z = (i * uMult) + uAdd;
		uAdj = count_adj(pBotList, &sBuf);
		if(uAdj > uMax)
		{
			uMax = uAdj;
			sMax = sBuf;
		}
		printf("uMax %lu, uAdj %lu\n", uMax, uAdj);
	}
	printf("%ld %ld %ld %lu\n", uMax, sMax.sPoint.x, sMax.sPoint.y, sMax.sPoint.z);
	find_nearest(pBotList, &sMax);

	return 0;
}
