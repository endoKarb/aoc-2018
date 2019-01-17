//Advent of Code 2018 Day 13
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TRACK_SIZE 512
#define EMPTY -1

typedef enum{LEFT, STRAIGHT, RIGHT} eTurn;

typedef struct{
	unsigned int x;
	unsigned int y;
} tCoor;

typedef struct{
	unsigned int ID;
	unsigned char dir;
	eTurn turn;
	tCoor cor;
} tCart;


typedef struct{
	unsigned int ID;
	unsigned char type;
	signed char cart;
} tTile;

typedef struct{
	tCart * list[TRACK_SIZE];
	unsigned int size;
} tCartList;

typedef struct{
	tTile * track[TRACK_SIZE][TRACK_SIZE];
	tCartList cart_list;
} tTrack;

tCart *
add_cart(char dir)
{
	static tCart carts_bank[TRACK_SIZE / 4];
	static unsigned int i = EMPTY;
	++i;

	switch(dir)
	{
	case '^':
		carts_bank[i].ID = i;
		carts_bank[i].dir = 0;
		break;
	case '>':
		carts_bank[i].ID = i;
		carts_bank[i].dir = 1;
		break;
	case 'v':
		carts_bank[i].ID = i;
		carts_bank[i].dir = 2;
		break;
	case '<':
		carts_bank[i].ID = i;
		carts_bank[i].dir = 3;
		break;
	default:
		puts("###INVALID CHARACTER###");
	}

	//printf("ID:%u dir:%c\n", carts_bank[i].ID, carts_bank[i].dir);
	return &carts_bank[i];
}

tTile *
add_tile(char type)
{
	static tTile tiles_bank[TRACK_SIZE * TRACK_SIZE];
	static unsigned int i = EMPTY;
	++i;

	switch(type)
	{
	case '-':
		tiles_bank[i].ID = i;
		tiles_bank[i].type = type;
		tiles_bank[i].cart = EMPTY;
		break;
	case '|':
		tiles_bank[i].ID = i;
		tiles_bank[i].type = type;
		tiles_bank[i].cart = EMPTY;
		break;
	case '/':
		tiles_bank[i].ID = i;
		tiles_bank[i].type = type;
		tiles_bank[i].cart = EMPTY;
		break;
	case '\\':
		tiles_bank[i].ID = i;
		tiles_bank[i].type = type;
		tiles_bank[i].cart = EMPTY;
		break;
	case '+':
		tiles_bank[i].ID = i;
		tiles_bank[i].type = type;
		tiles_bank[i].cart = EMPTY;
		break;
	case 'v':
		tiles_bank[i].ID = i;
		tiles_bank[i].type = '|';
		tiles_bank[i].cart = 2;
		break;
	case '^':
		tiles_bank[i].ID = i;
		tiles_bank[i].type = '|';
		tiles_bank[i].cart = 0;
		break;
	case '>':
		tiles_bank[i].ID = i;
		tiles_bank[i].type = '-';
		tiles_bank[i].cart = 1;
		break;
	case '<':
		tiles_bank[i].ID = i;
		tiles_bank[i].type = '-';
		tiles_bank[i].cart = 3;
		break;
	default:
		puts("###INVALID CHARACTER###");
	}

	//printf("ID:%u type:%c cart:%p\n", tiles_bank[i].ID, tiles_bank[i].type, tiles_bank[i].cart);
	//printf("i:%d, p:%p\n", i, &tiles_bank[i]);
	return &tiles_bank[i];
}

void
parse_input(tTrack * pTrack)
{
	char ch;
	unsigned int row = 0;
	tTile ** scan;
	tCart * new_cart;

	for(unsigned int col = 0; (ch=getchar()) != EOF; ++col)
	{
		scan = &pTrack -> track[row][col];
		if(ch == '\n')
		{
			col = EMPTY;
			++row;
		}
		else if(ch != ' ')
		{
			*scan = add_tile(ch);
			if((*scan) -> cart != EMPTY)
			{
				new_cart = add_cart(ch);
				new_cart -> cor.x = col;
				new_cart -> cor.y = row;

				pTrack -> cart_list.list[pTrack -> cart_list.size] = new_cart;
				++pTrack -> cart_list.size;
			}
			/*
			printf("row:%u, col:%u char:%u\n", row, col, ch);
			printf("scan:%p, *scan:%p\n", scan, *scan);
			*/
		}
		//printf("row:%u, col:%u p:%p\n", row, col, *scan);
		//putchar(ch);
	}

}

char
cart_dir(unsigned char dir)
{
	switch(dir)
	{
	case 0:
		return '^';
	case 1:
		return '>';
	case 2:
		return 'v';
	case 3:
		return '<';
	default:
		return '?';
	}
}

void
print_track(tTrack * pTrack)
{
	bool empty = true;
	tTile * scan;
	for(int i = 0; i < TRACK_SIZE; ++i)
	{
		printf("%3u ", i);
		empty = true;
		for(int j = 0; j < TRACK_SIZE; ++j)
		{
			scan = pTrack -> track[i][j];
			//printf("i:%d, j:%d\n", i, j);
			if(scan)
			{
				empty = false;
				if(scan -> cart != EMPTY)
					putchar(cart_dir(scan -> cart));
				else
					putchar(scan -> type);
			}
			else
			{
				putchar(' ');
			}
		}

		//stop at empty line
		if(!empty)
		{
			putchar('\n');
		}
		else
		{
			putchar('\n');
			break;
		}
	}
}

bool
collision(tTile * pDest)
{
	if(pDest -> cart != EMPTY)
		return true;
	else
		return false;
}

tTile *
destination(tTrack * pTrack, tCart * pCart)
{
	static unsigned int x, y;
	tTile * pDest;

	x = pCart -> cor.x;
	y = pCart -> cor.y;

	if(pCart->ID == 19)
	{
		printf("ID:%u, y%d, x%d, dir:%c, tile:%c\n",
			pCart -> ID, y, x, cart_dir(pCart -> dir),
			pTrack -> track[y][x] -> type);
	}
	switch(pCart -> dir)
	{
	case 0:
		pDest = pTrack -> track[y - 1][x];
		break;
	case 1:
		pDest = pTrack -> track[y][x + 1];
		break;
	case 2:
		pDest = pTrack -> track[y + 1][x];
		break;
	case 3:
		pDest = pTrack -> track[y][x - 1];
		break;
	default:
		puts("INVALID DIR");
	}

	//printf("desttype:%c\n", pDest -> type);
	return pDest;
}

void
move_cart(tCart * pCart)
{
	switch(pCart -> dir)
	{
	case 0:
		--pCart -> cor.y;
		break;
	case 1:
		++pCart -> cor.x;
		break;
	case 2:
		++pCart -> cor.y;
		break;
	case 3:
		--pCart -> cor.x;
		break;
	default:
		puts("Invalid Direction");
	}
}

void
turn_right(unsigned char * pDir)
{
	*pDir += 1;
	*pDir %= 4;
}

void
turn_left(unsigned char * pDir)
{
	*pDir += 3;
	*pDir %= 4;
}

void
turn(tCart * pCart, const char tile)
{
	unsigned char * pDir = &pCart -> dir;
	//printf("turn()\ntile:%c, pDir:%u\n", tile, *pDir);
	switch(tile)
	{
	case '\\':
		//0 = vertical, 1 = horizontal
		if(*pDir % 2 == 0)
			turn_left(pDir);
		else if(*pDir % 2 == 1)
			turn_right(pDir);
		break;
	case '/':
		//0 = vertical, 1 = horizontal
		if(*pDir % 2 == 0)
			turn_right(pDir);
		else if(*pDir % 2 == 1)
			turn_left(pDir);
		break;
	case '+':
		//printf("turn:%d\n", pCart -> turn);
		switch(pCart -> turn)
		{
		case LEFT:
			turn_left(pDir);
			pCart -> turn = STRAIGHT;
			break;
		case STRAIGHT:
			pCart -> turn = RIGHT;
			break;
		case RIGHT:
			turn_right(pDir);
			pCart -> turn = LEFT;
			break;
		}
		break;
	default:
		puts("CAN'T TURN");
	}
}

int
lowest_cor(const void * v1, const void * v2)
{
	static int r;
	static tCart ** c1;
	static tCart ** c2;
	static tCart * cart1;
	static tCart * cart2;

	c1 = (tCart **) v1;
	c2 = (tCart **) v2;

	cart1 = *c1;
	cart2 = *c2;

	r = cart1 -> cor.y - cart2 -> cor.y;
	if(r == 0)
		r = cart1 -> cor.x - cart2 -> cor.x;
	
	return r;
}

void
print_cart_list(tCartList * pList)
{
	tCart * scan;
	for(int i = 0; i < pList -> size; ++i)
	{
		scan = pList -> list[i];
		//printf("i:%u, p:%p\n", i, scan);
		printf("ID:%u, y:%u, x:%u\n", scan -> ID, scan -> cor.y, scan -> cor.x);
	}
	putchar('\n');
}

bool
tick(tTrack * pTrack)
{
	static tCartList * pList;
	static tTile * pDest;
	static tTile * pSrc;
	static tCart * pCart;
	static char tile;

	pList = &pTrack -> cart_list;

	for(int i = 0; i < pList -> size; ++i)
	{
		pCart = pList -> list[i];
		pSrc = pTrack -> track[pCart -> cor.y][pCart -> cor.x];
		pDest = destination(pTrack, pCart);

		if(!collision(pDest))
		{
			//move cart
			move_cart(pCart);
			//update map
			pSrc -> cart = EMPTY;
		}
		else
		{
			move_cart(pCart);
			pSrc -> cart = EMPTY;
			//break and report
			printf("COLLISION!\nx:%u, y:%u\n", pCart -> cor.x, pCart -> cor.y);
			return false;
		}

		tile = pDest -> type;
		if(tile != '-' && tile != '|')
		{
			//turning
			turn(pCart, tile);
		}
		pDest -> cart = pCart -> dir;
		
/*
*/
	}
	//sort karts;
	qsort(pList -> list, pList -> size, sizeof(pList -> list[0]), lowest_cor);
	print_cart_list(pList);
	//return true;
	return true;
}

int main(void)
{
	tTrack track;
	memset(&track, 0, sizeof(tTrack));
	parse_input(&track);

	//print_track(&track);

	for(int i = 0; tick(&track); ++i)
	{
		//print_track(&track);
	}
/*
*/
	return 0;
}
