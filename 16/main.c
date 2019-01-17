//Advent of Code 2018 Day 16
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SAMPLE_SIZE (1 << 15)
#define MATCHES_SIZE (1 << 5)

typedef struct{
	unsigned char vHead[4];
} tReg;

typedef struct{
	tReg before;
	tReg after;
	tReg instr;
	unsigned char count;
} tSample;

typedef struct{
	tSample vHead[SAMPLE_SIZE];
	unsigned short size;
} tSampleList;

void
print_reg(const tReg reg)
{
	for(int i = 0; i < 4; ++i)
	{
		printf("%u ", reg.vHead[i]);
	}
	putchar('\n');
}

void
parse_input(tSampleList * pSampleList)
{
	tReg before;
	tReg after;
	tReg instr;

	int ch;
	while((ch = getchar()) != EOF)
	{
		switch(ch)
		{
			case 'A':
				scanf("fter: [%hhu, %hhu, %hhu, %hhu]\n",
				&after.vHead[0], &after.vHead[1], &after.vHead[2], &after.vHead[3]);
				tSample buf = {before, after, instr};
				pSampleList -> vHead[pSampleList -> size] = buf;
				pSampleList -> size += 1;
				break;
			case 'B':
				scanf("efore: [%hhu, %hhu, %hhu, %hhu]\n",
				&before.vHead[0], &before.vHead[1], &before.vHead[2], &before.vHead[3]);
				scanf("%hhu %hhu %hhu %hhu\n",
				&instr.vHead[0], &instr.vHead[1], &instr.vHead[2], &instr.vHead[3]);
				
				break;
			default:
				break;
		}
	}
	
}


unsigned char
check_all(const tSample * pSample)
{

	static tReg before;
	static tReg after;
	static tReg instr;
	static tReg buf;

	before = pSample -> before;
	after = pSample -> after;
	instr = pSample -> instr;


	unsigned char tot = 0;

	putchar('\n');
	print_reg(before);
	print_reg(instr);
	print_reg(after);


	//addr
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] + before.vHead[instr.vHead[2]];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match addr");
	}

	//addi
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] + instr.vHead[2];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match addi");
	}

	//mulr
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] * before.vHead[instr.vHead[2]];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match mulr");
	}

	
	
	//muli
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] * instr.vHead[2];
	print_reg(buf);

	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match muli");
	}
	

	//banr
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] & before.vHead[instr.vHead[2]];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match banr");
	}
	

	//bani
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] & instr.vHead[2];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match bani");
	}


	//borr
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] | before.vHead[instr.vHead[2]];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match borr");
	}
	
	//bori
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] | instr.vHead[2];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match bori");
	}

	//setr
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match setr");
	}

	
	//seti
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = instr.vHead[1];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match seti");
	}

	//gtir
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = instr.vHead[1] > before.vHead[instr.vHead[2]];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match gtir");
	}

	//gtri
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] > instr.vHead[2];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match gtri");
	}

	//gtrr
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] > before.vHead[instr.vHead[2]];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match gtrr");
	}
	
	//eqir
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = instr.vHead[1] == before.vHead[instr.vHead[2]];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match eqir");
	}

	//eqri
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] == instr.vHead[2];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match eqri");
	}

	//eqrr
	buf = pSample -> before;
	buf.vHead[instr.vHead[3]] = before.vHead[instr.vHead[1]] == before.vHead[instr.vHead[2]];
	print_reg(buf);
	if(!memcmp(&buf, &pSample -> after, sizeof(tReg)))
	{
		++tot;
		puts("match eqrr");
	}


	//printf("%u\n", buf.vHead[instr.vHead[3]]);

	return tot;
}

unsigned char
test_every_op(tSample * pSample)
{
	unsigned char tot = 0;
	tot += check_all(pSample);
	return tot;
}

int main(void)
{
	tSampleList * pSampleList = (tSampleList *) calloc(1, sizeof(tSampleList));

	//parse_input
	parse_input(pSampleList);

	//for every instrr
	unsigned short answer = 0;
	tSample * pSample;
	for(int i = 0; i < pSampleList -> size; ++i)
	//for(int i = 0; i < 6; ++i)
	{
		pSample = &pSampleList -> vHead[i];

		pSample -> count = test_every_op(pSample);

		//test every operation, return how many "fit"
		if(pSample -> count < 3)
			printf("Sample %i matches %hu operations\n", i, pSample -> count);
		if(pSample -> count >= 3)
			++answer;
	}

	printf("%hu samples match 3 or more ops.\n", answer);
	
	return 0;
}
