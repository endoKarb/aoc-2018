//Advent of Code 2018 Day 8.
#include <stdio.h>
#include <stdlib.h>
#define SIZE 16

typedef struct node{
	unsigned int id;
	struct node * child_nodes[SIZE];
	unsigned int n_of_childs;
	unsigned int meta[SIZE];
	unsigned int n_of_meta;
	struct node * parent_node;

} Node;

Node * BASE;
unsigned int NAME = 1;
unsigned int LEVEL = 0;
unsigned int MAX_LEVEL = 0;
unsigned int MAX_CHILD = 0;
unsigned int ANS;

Node *
add_child(Node * parent)
{
	Node * new = (Node *) calloc(1, sizeof(Node));
	new->id = NAME;
	++NAME;
	++LEVEL;
	if(LEVEL > MAX_LEVEL)
		MAX_LEVEL = LEVEL;

	scanf("%u %u", &new->n_of_childs, &new->n_of_meta);
	if(new->n_of_childs > MAX_CHILD)
		MAX_CHILD = new->n_of_childs;

	new->parent_node = parent;
	
	int i = 1;
	while(i <= new->n_of_childs)
	{
		new->child_nodes[i] = add_child(new);
		++i;
	}

	i = 0;
	unsigned int buf;
	while(i < new->n_of_meta)
	{
		scanf("%u", &buf);
		new->meta[i] = buf;
		ANS += new->meta[i];
		++i;
	}
	
	--LEVEL;
	return new;
}

unsigned int
calc_value(Node * node)
{
	unsigned int ans = 0;

	if(!node->n_of_childs)
	{
		for(int i = 0; i < node->n_of_meta; ++i)  
		{
			ans += node->meta[i];
		}
	}
	else
	{
		for(int i = 0; i < node->n_of_meta; ++i)  
		{
			if(node->meta[i] <= node->n_of_childs && node->meta[i] != 0)
			{
				ans += calc_value(node->child_nodes[node->meta[i]]);
			}
			//printf("name: %c, meta: %d, ans: %d\n", node->name, node->meta[i], ans);
		}
	}
	return ans;
}

int main(void)
{
	BASE = add_child(NULL);
	printf("The answer to the first part is %d.\n", ANS);
	ANS = calc_value(BASE);
	printf("The answer to the second part is %d.\n", ANS);
	printf("The tree is %d meters high.\n", MAX_LEVEL);
	printf("The largest brach has %d branches\n", MAX_CHILD);
	return 0;
}
