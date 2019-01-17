//Advent of Code 2018 Day 16 Part 2
#include <stdio.h>
int main(void)
{
	unsigned short reg[4] = {0, 0, 0, 0};
	unsigned short opcode = 0;
	unsigned short A = 0;
	unsigned short B = 0;
	unsigned short C = 0;

	while(scanf("%hu %hu %hu %hu\n", &opcode, &A, &B, &C) == 4)
	{
		printf("register: %hu %hu %hu %hu ", reg[0], reg[1], reg[2], reg[3]);
		switch(opcode)
		{
			case 0:
				reg[C] = reg[A] | reg[B];
				puts("borr");
				break;
			case 1:
				reg[C] = reg[A] + reg[B];
				puts("addr");
				break;
			case 2:
				reg[C] = reg[A] == reg[B];
				puts("eqrr");
				break;
			case 3: 
				reg[C] = reg[A] + B;
				puts("addi");
				break;
			case 4:
				reg[C] = reg[A] == B;
				puts("eqri");
				break;
			case 5:
				reg[C] = A == reg[B];
				puts("eqir");
				break;
			case 6:
				reg[C] = reg[A] > B;
				puts("gtri");
				break;
			case 7:
				reg[C] = reg[A] * reg[B];
				puts("mulr");
				break;
			case 8:
				reg[C] = reg[A];
				puts("setr");
				break;
			case 9:
				reg[C] = A > reg[B];
				puts("gtir");
				break;
			case 10:
				reg[C] = reg[A] * B;
				puts("muli");
				break;
			case 11:
				reg[C] = reg[A] & reg[B];
				puts("banr");
				break;
			case 12:
				reg[C] = A;
				puts("seti");
				break;
			case 13:
				reg[C] = reg[A] > reg[B];
				puts("gtrr");
				break;
			case 14:
				reg[C] = reg[A] & B;
				puts("bani");
				break;
			case 15:
				reg[C] = reg[A] | B;
				puts("bori");
				break;
			default:
				puts("INVALID OPCODE");
				break;
		}
	}
	printf("register: %hu %hu %hu %hu\n", reg[0], reg[1], reg[2], reg[3]);
	return 0;
}
