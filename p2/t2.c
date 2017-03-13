#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define N(a) (int) (sizeof(a)/sizeof(a[0]))

struct my_node {
	int key;
	int data;
	struct tree_node tree;
};

static int pa[] = {5,4,9,2,6,7,15};

int my_node_cmp(const struct tree_node *x, const struct tree_node *y)
{
	const struct my_node *p = tree_entry(x, struct my_node, tree);
	const struct my_node *q = tree_entry(y, struct my_node, tree);
	if (p->key == q->key)
		return 0;
	else if (p->key < q->key)
		return 1;
	else
		return -1;
}

void my_node_print(const struct tree_node *x)
{
	const struct my_node *p = tree_entry(x, struct my_node, tree);
	printf("key = %d, data = %d\n", p->key, p->data);	
}

void my_node_free(const struct tree_node *x)
{
	free(tree_entry(x, struct my_node, tree));
}

int main(void)
{
	struct my_node *root, *p, hint;
	struct tree_node *pt;
	int i;
	root = malloc(sizeof(struct my_node));
	root->key = pa[0];
	root->data = 0;
	tree_init(&root->tree);

	for (i=1; i<N(pa); ++i) {
		p = malloc(sizeof(struct my_node));
		p->key = pa[i];
		p->data = i;
		tree_init(&p->tree);
		tree_insert(&root->tree, &p->tree, my_node_cmp);
	}

	hint.key = 9;
	pt = tree_search(&root->tree, &hint.tree, my_node_cmp);
	tree_del(&root->tree, pt, my_node_cmp);
	tree_foreach_in(&root->tree, my_node_print);
	tree_foreach_post(&root->tree, my_node_free);

	return 0;
}
