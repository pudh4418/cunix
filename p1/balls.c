#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "balls.h"

static const char *state_name[] = {"UN", "N", "MH", "ML", "H", "L"};

struct ball_index *ball_index_cpart(struct ball_index *bi, int start, int l)
{
	struct ball_index *p;
	int i;
	p = malloc(sizeof(struct ball_index));
	p->n = l;
	for (i = 0; i < l; ++i)
		p->a[i] = bi->a[i + start];
	return p;
}

struct ball_index *ball_index_get_n_normal(struct balls *b, int n)
{
	struct ball_index *p;
	int i, j;
	p = malloc(sizeof(struct ball_index));
	p->n = n;
	i = 0;
	j = 0;
	while (n--) {
		while (b[i].state != NORMAL)
			++i;
		p->a[j++] = i;
		++i;
	}
	return p;
}

struct ball_index *ball_index_make_ncballs(int x, int n)
{
	struct ball_index *p;
	int i;
	p = malloc(sizeof(struct ball_index));
	p->n = n;
	for (i = 0; i < n; ++i)
		p->a[i] = i + x;
	return p;
}

struct ball_index *ball_index_add(struct ball_index *bi, struct ball_index *bx)
{
	struct ball_index *p;
	int i;
	p = malloc(sizeof(struct ball_index));
	memcpy(p, bi, sizeof(struct ball_index));
	for (i = 0; i < bx->n; ++i)
		p->a[p->n + i] = bx->a[i];
	p->n += bx->n;
	return p;
}

int ball_weight(struct balls *b, struct ball_index *bi)
{
	int i, ans = 0;
	for (i = 0; i < bi->n; ++i)
		ans += b[bi->a[i]].w;
	return ans;
}

void ball_update_state(struct balls *b, struct ball_index *bi, enum ball_state state)
{
	int i;
	for (i = 0; i < bi->n; ++i)
		b[bi->a[i]].state = state;
}

void ball_show_state(struct balls *b)
{
	int i;
	puts(" bi  0  1  2  3  4  5  6  7  8  9 10 11");
	printf("  s");
	for (i = 0; i < 12; ++i)
		printf("%3s", state_name[b[i].state]);
	printf("\n");
}

