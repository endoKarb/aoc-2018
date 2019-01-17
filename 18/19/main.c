//Advent of Code 2018 Day 19
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGISTER_SIZE 6
#define MAX_INSTR (1 << 6)

typedef struct {
	unsigned long vHead[REGISTER_SIZE];
} tRegister;

typedef enum {
	addr, addi, mulr, muli, setr, seti, eqrr, gtrr
} eInstrType;

typedef struct {
	eInstrType eType;
	unsigned char vHead[3];
} tInstr;

typedef struct {
	unsigned short size;
	tInstr vHead[MAX_INSTR];
} tInstrList;

unsigned char IP;

eInstrType
string_to_enum(char vBuf[])
{
	if(!strcmp(vBuf, "addr"))
		return addr; 
	if(!strcmp(vBuf, "addi"))
		return addi; 
	if(!strcmp(vBuf, "mulr"))
		return mulr; 
	if(!strcmp(vBuf, "muli"))
		return muli; 
	if(!strcmp(vBuf, "setr"))
		return setr; 
	if(!strcmp(vBuf, "seti"))
		return seti; 
	if(!strcmp(vBuf, "eqrr"))
		return eqrr; 
	if(!strcmp(vBuf, "gtrr"))
		return gtrr; 

	return 99;
}

void
parse_input(tInstrList * pInstrList)
{
	tInstr * vHead = pInstrList -> vHead;
	int ch;
	char vBuf[] = "aaaa";

	while((ch = getchar()) != EOF)
	//for(int i = 0; (ch = getchar()) != EOF; ++i)
	{
		switch(ch)
		{
			case '#':
				scanf("ip %hhu", &IP);
				break;
			default:
				scanf("%s %hhu %hhu %hhu", vBuf,
					&vHead[pInstrList -> size].vHead[0],
					&vHead[pInstrList -> size].vHead[1],
					&vHead[pInstrList -> size].vHead[2]);
				vHead[pInstrList -> size].eType = string_to_enum(vBuf);
				printf("%hhu %hhu %hhu %hhu, size: %u\n",
					vHead[pInstrList -> size].eType,
					vHead[pInstrList -> size].vHead[0],
					vHead[pInstrList -> size].vHead[1],
					vHead[pInstrList -> size].vHead[2],
					pInstrList -> size);
				++pInstrList -> size;
				break;
		}
	}
}

void
print_register(tRegister * pReg)
{
	for(int i = 0; i < REGISTER_SIZE; ++i)
	{
		printf("%d:[%010lu]   ", i, pReg -> vHead[i]);
	}
	putchar('\n');
}

void
print_instr(const tInstr * pInstr, const unsigned short index)
{
	char * type;
	switch(pInstr -> eType)
	{
		case addi:
			type = "addi";
			break;
		case addr:
			type = "addr";
			break;
		case muli:
			type = "muli";
			break;
		case mulr:
			type = "mulr";
			break;
		case seti:
			type = "seti";
			break;
		case setr:
			type = "setr";
			break;
		case eqrr:
			type = "eqrr";
			break;
		case gtrr:
			type = "gtrr";
			break;
		default:
			type = "ERROR";
			break;
	}
	printf("Instruction %hhu: %s %hhu %hhu %hhu\n",
			index, type,
			pInstr -> vHead[0],
			pInstr -> vHead[1],
			pInstr -> vHead[2]);
}

void
do_addr(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] + pReg -> vHead[B];
}

void
do_mulr(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] * pReg -> vHead[B];
}

void
do_muli(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] * B;
}

void
do_addi(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] + B;
}

void
do_setr(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	//static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	//B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A];
}

void
do_seti(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	//static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	//B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = A;
}

void
do_eqrr(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] == pReg -> vHead[B];
}

void
do_gtrr(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] > pReg -> vHead[B];
}

void
exec_instr(tRegister * pReg, tInstrList * pIList)
{
	static const tInstr * pCurr;

	pCurr = &pIList -> vHead[pReg -> vHead[IP]];

//	print_instr(pCurr, pReg -> vHead[IP]);

	switch(pCurr -> eType)
	{
		case addr:
			do_addr(pReg, pCurr);
			break;
		case addi:
			do_addi(pReg, pCurr);
			break;
		case mulr:
			do_mulr(pReg, pCurr);
			break;
		case muli:
			do_muli(pReg, pCurr);
			break;
		case setr:
			do_setr(pReg, pCurr);
			break;
		case seti:
			do_seti(pReg, pCurr);
			break;
		case eqrr:
			do_eqrr(pReg, pCurr);
			break;
		case gtrr:
			do_gtrr(pReg, pCurr);
			break;
		default:
			printf("ILLEGAL FUNCTION %u\n", pCurr -> eType);
	}

	++pReg -> vHead[IP];
	//print_register(pReg);

	static unsigned long * scan;
	//static unsigned long buffer = 0;
	scan = &pReg -> vHead[0];

	static unsigned long buff = 0;
	if(buff != scan[0])
	{
		puts("boop");
		print_register(pReg);
		buff = scan[0];
	}

	if(scan[3] == 3)
	{
		if(scan[1] % scan[5] == 0)
		{
			if(scan[1] / scan[5] > scan[4])
			{
				scan[4] = scan[1] / scan[5];
			}
		}
	}
	if(scan[3] == 8)
	{
		if(scan[1] > scan[4])
		{
			scan[4] = scan[1];
		}
	}

}

int main(void)
{
	tRegister * pReg = (tRegister *) calloc(1, sizeof(tRegister));
	tInstrList * pInstrList = (tInstrList *) calloc(1, sizeof(tInstrList));
	parse_input(pInstrList);

	pReg -> vHead[0] = 1;

	//for(unsigned long i = 0; /**/ i < 1000 && pReg -> vHead[IP] < pInstrList -> size; ++i)
	for(unsigned long i = 0; pReg -> vHead[5] <= pReg -> vHead[1]; ++i)
	{
		exec_instr(pReg, pInstrList);
	}
	print_register(pReg);

/*
	unsigned long f = 0;
	printf("%lu\n", f - 1);
	while(pReg -> vHead[IP] < pInstrList -> size)
	{
		exec_instr(pReg, pInstrList);
	}
*/
	return 0;
}
