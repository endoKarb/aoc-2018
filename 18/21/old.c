//Advent of Code 2018 Day 19
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define REGISTER_SIZE 6
#define MAX_INSTR (1 << 6)

typedef struct {
	unsigned long vHead[REGISTER_SIZE];
} tRegister;

typedef enum {
	addr, addi,
	mulr, muli,
	banr, bani,
	borr, bori,
	setr, seti,
	gtir, gtri, gtrr,
	eqir, eqri, eqrr
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
	if(!strcmp(vBuf, "banr"))
		return banr; 
	if(!strcmp(vBuf, "bani"))
		return bani; 
	if(!strcmp(vBuf, "borr"))
		return borr; 
	if(!strcmp(vBuf, "bori"))
		return bori; 
	if(!strcmp(vBuf, "setr"))
		return setr; 
	if(!strcmp(vBuf, "seti"))
		return seti; 
	if(!strcmp(vBuf, "gtir"))
		return gtir; 
	if(!strcmp(vBuf, "gtri"))
		return gtri; 
	if(!strcmp(vBuf, "gtrr"))
		return gtrr; 
	if(!strcmp(vBuf, "eqir"))
		return eqir; 
	if(!strcmp(vBuf, "eqri"))
		return eqri; 
	if(!strcmp(vBuf, "eqrr"))
		return eqrr; 
	else
		assert(false);
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
		case addr:
			type = "addr";
			break;
		case addi:
			type = "addi";
			break;
		case mulr:
			type = "mulr";
			break;
		case muli:
			type = "muli";
			break;
		case banr:
			type = "banr";
			break;
		case bani:
			type = "bani";
			break;
		case borr:
			type = "borr";
			break;
		case bori:
			type = "bori";
			break;
		case setr:
			type = "setr";
			break;
		case seti:
			type = "seti";
			break;
		case gtir:
			type = "gtir";
			break;
		case gtri:
			type = "gtri";
			break;
		case gtrr:
			type = "gtrr";
			break;
		case eqir:
			type = "eqir";
			break;
		case eqri:
			type = "eqri";
			break;
		case eqrr:
			type = "eqrr";
			break;
		default:
			assert(false);
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
do_banr(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] & pReg -> vHead[B];
}

void
do_bani(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] & B;
}

void
do_borr(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] | pReg -> vHead[B];
}

void
do_bori(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] | B;
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
do_gtir(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = A > pReg -> vHead[B];
}

void
do_gtri(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] > B;
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
do_eqir(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = A == pReg -> vHead[B];
}

void
do_eqri(tRegister * pReg, const tInstr * pInstr)
{
	static unsigned short A;
	static unsigned short B;
	static unsigned short C;

	A = pInstr -> vHead[0];
	B = pInstr -> vHead[1];
	C = pInstr -> vHead[2];

	pReg -> vHead[C] = pReg -> vHead[A] == B;
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
exec_instr(tRegister * pReg, tInstrList * pIList)
{
	static const tInstr * pCurr;

	pCurr = &pIList -> vHead[pReg -> vHead[IP]];

	print_instr(pCurr, pReg -> vHead[IP]);

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
		case banr:
			do_banr(pReg, pCurr);
			break;
		case bani:
			do_bani(pReg, pCurr);
			break;
		case borr:
			do_borr(pReg, pCurr);
			break;
		case bori:
			do_bori(pReg, pCurr);
			break;
		case setr:
			do_setr(pReg, pCurr);
			break;
		case seti:
			do_seti(pReg, pCurr);
			break;
		case gtir:
			do_gtir(pReg, pCurr);
			break;
		case gtri:
			do_gtri(pReg, pCurr);
			break;
		case gtrr:
			do_gtrr(pReg, pCurr);
			break;
		case eqir:
			do_eqir(pReg, pCurr);
			break;
		case eqri:
			do_eqri(pReg, pCurr);
			break;
		case eqrr:
			do_eqrr(pReg, pCurr);
			break;
		default:
			assert(false);
			//printf("ILLEGAL FUNCTION %u\n", pCurr -> eType);
	}

	++pReg -> vHead[IP];
	//print_register(pReg);

}

int main(void)
{
	tRegister * pReg = (tRegister *) calloc(1, sizeof(tRegister));
	tInstrList * pInstrList = (tInstrList *) calloc(1, sizeof(tInstrList));
	parse_input(pInstrList);

	pReg -> vHead[0] = 1;

	for(unsigned long i = 0; i < 10000; ++i)
	{
		exec_instr(pReg, pInstrList);
		print_register(pReg);
	}

	return 0;
}
