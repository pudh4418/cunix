#include <stdlib.h>
#include "tree.h"

void tree_init(struct tree_node *root)
{
	root->left = NULL;
	root->right = NULL;
	root->parent = NULL;
}

void tree_insert(struct tree_node *root, struct tree_node *node, tree_cmpfun fun)
{
	if (fun(root, node) <= 0) {
		if (root->left == NULL) {
			root->left = node;
			node->parent = root;
		}
		else
			tree_insert(root->left, node, fun);
	}
	else {
		if (root->right == NULL) {
			root->right = node;
			node->parent = root;
		}
		else
			tree_insert(root->right, node, fun);
	}
}

struct tree_node *tree_search(struct tree_node *root, struct tree_node *node, tree_cmpfun fun)
{
	if (root == NULL)
		return NULL;
	if (fun(root, node) == 0)
		return root;
	else if (fun(root, node) < 0)
		return tree_search(root->left, node, fun);
	else
		return tree_search(root->right, node, fun);
}

void tree_foreach_pre(struct tree_node *root, tree_trvfun fun)
{
	if (root == NULL)
		return;
	fun(root);
	tree_foreach_pre(root->left, fun);
	tree_foreach_pre(root->right, fun);
}

void tree_foreach_in(struct tree_node *root, tree_trvfun fun)
{
	if (root == NULL)
		return;
	tree_foreach_in(root->left, fun);
	fun(root);
	tree_foreach_in(root->right, fun);
}

void tree_foreach_post(struct tree_node *root, tree_trvfun fun)
{
	if (root == NULL)
		return;
	tree_foreach_post(root->left, fun);
	tree_foreach_post(root->right, fun);
	fun(root);
}

static struct tree_node *tree_min(struct tree_node *x)
{
	while (x->left != NULL)
		x = x->left;
	return x;
}

static void tree_modpar(struct tree_node *x, struct tree_node *y)
{
	if (x->parent == NULL)
		return;
	if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	if (y != NULL)
		y->parent = x->parent;
}

void tree_del(struct tree_node *root, struct tree_node *hint, tree_cmpfun fun)
{
	if (root == NULL)
		return;
	if (fun(root, hint) < 0)
		tree_del(root->left, hint, fun);
	else if (fun(root, hint) > 0)
		tree_del(root->right, hint, fun);
	else
	{
		if (root->left != NULL && root->right != NULL) {
			struct tree_node *p = tree_min(root->right);
			tree_modpar(p, p->right);
			p->left = root->left;
			p->right = root->right;
			tree_modpar(root, p);
		}	
		else if (root->left != NULL)
			tree_modpar(root, root->left);
		else if (root->right != NULL)
			tree_modpar(root, root->right);
		else
			tree_modpar(root, NULL);
	}
}

