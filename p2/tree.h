#ifndef TREE_H
#define TREE_H

struct tree_node {
	struct tree_node *left, *right, *parent;
};

typedef int (*tree_cmpfun)(const struct tree_node *, const struct tree_node *);
typedef void (*tree_trvfun)(const struct tree_node *);

#define tree_entry(ptr, type, member) container_of(ptr, type, member)
#define container_of(ptr, type, member) \
	(type *)((char *)ptr - offsetof(type, member))
#define offsetof(type, member) ((size_t) &((type *)0)->member)

void tree_init(struct tree_node *);
void tree_insert(struct tree_node *root, struct tree_node *node, tree_cmpfun fun);
struct tree_node *tree_search(struct tree_node *root, struct tree_node *node, tree_cmpfun fun);
void tree_foreach_pre(struct tree_node *root, tree_trvfun fun);
void tree_foreach_in(struct tree_node *root, tree_trvfun fun);
void tree_foreach_post(struct tree_node *root, tree_trvfun fun);
void tree_del(struct tree_node *root, struct tree_node *node, tree_cmpfun fun);


#endif

