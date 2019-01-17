//Advent of Code 2018 Day 8.
#include <stdio.h>
#include <stdlib.h>
#define SIZE 128

typedef struct node{
	char name;
	struct node * child_nodes[SIZE];
	struct node * parent_node;

} Node;

Node * BASE;
unsigned int ANS;

Node *
add_child(Node * parent)
{
	unsigned int childs;
	unsigned int meta;

	Node * new = (Node *) calloc(1, sizeof(Node));

	scanf("%u %u", &childs, &meta);

	new->parent_node = parent;
	
	int i = 1;
	while(i <= childs)
	{
		new->child_nodes[i] = add_child(new);
		++i;
	}

	unsigned int buf;
	while(meta > 0)
	{
		scanf("%u", &buf);
		ANS += buf;
		--meta;
	}

	return new;
}

int main(void)
{
	BASE = add_child(NULL);
	printf("The answer is %d.\n", ANS);
	return 0;
}
