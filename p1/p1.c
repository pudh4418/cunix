#include <stdio.h>
#include <stdlib.h>
#include "balls.h"

static void print_compare(struct ball_index *ba, struct ball_index *bb)
{
	int i;
	printf("Compare ");
	for (i = 0; i < ba->n; ++i)
		printf("%d ", ba->a[i]);
	printf("to ");
	for (i = 0; i < bb->n; ++i)
		printf("%d ", bb->a[i]);
	printf(".\n");
}

static int get_1f1(struct balls *b, struct ball_index *bs)
{
	struct ball_index *bc;
	int ans;
	bc = ball_index_get_n_normal(b, 1);

	ball_show_state(b);
	print_compare(bs, bc);
	ans = bs->a[0];
	if (ball_weight(b, bs) > ball_weight(b, bc))
		ball_update_state(b, bs, HEAVY);
	else if (ball_weight(b, bs) < ball_weight(b, bc))
		ball_update_state(b, bs, LIGHT);
	else
		fputs("Error!", stderr);
	free(bc);
	return ans;
}

static int get_1f3_l(struct balls *b, struct ball_index *bs)
{
	struct ball_index *ba, *bb, *bc;
	int ans;
	ba = ball_index_cpart(bs, 0, 1);
	bb = ball_index_cpart(bs, 1, 1);
	bc = ball_index_cpart(bs, 2, 1);

	ball_show_state(b);
	print_compare(ba, bb);
	if (ball_weight(b, ba) == ball_weight(b, bb)) {
		ball_update_state(b, ba, NORMAL);
		ball_update_state(b, bb, NORMAL);
		ball_update_state(b, bc, LIGHT);
		ans  = bc->a[0];
	}
	else {
		ball_update_state(b, bc, NORMAL);
		if (ball_weight(b, ba) < ball_weight(b, bb)) {
			ball_update_state(b, ba, LIGHT);
			ball_update_state(b, bb, NORMAL);
			ans = ba->a[0];
		}
		else {
			ball_update_state(b, bb, LIGHT);
			ball_update_state(b, ba, NORMAL);
			ans = bb->a[0];
		}
	}

	free(ba);
	free(bb);
	free(bc);
	return ans;
}

static int get_1f3_h(struct balls *b, struct ball_index *bs)
{
	struct ball_index *ba, *bb, *bc;
	int ans;
	ba = ball_index_cpart(bs, 0, 1);
	bb = ball_index_cpart(bs, 1, 1);
	bc = ball_index_cpart(bs, 2, 1);

	ball_show_state(b);
	print_compare(ba, bb);
	if (ball_weight(b, ba) == ball_weight(b, bb)) {
		ball_update_state(b, ba, NORMAL);
		ball_update_state(b, bb, NORMAL);
		ball_update_state(b, bc, HEAVY);
		ans  = bc->a[0];
	}
	else {
		ball_update_state(b, bc, NORMAL);
		if (ball_weight(b, ba) < ball_weight(b, bb)) {
			ball_update_state(b, ba, NORMAL);
			ball_update_state(b, bb, HEAVY);
			ans = bb->a[0];
		}
		else {
			ball_update_state(b, bb, NORMAL);
			ball_update_state(b, ba, HEAVY);
			ans = ba->a[0];
		}
	}

	free(ba);
	free(bb);
	free(bc);
	return ans;
}

static int get_1f4(struct balls *b, struct ball_index *bs)
{
	struct ball_index *ba, *bb, *bc;
	int ans;
	ba = ball_index_cpart(bs, 0, 3);
	bb = ball_index_cpart(bs, 3, 1);
	bc = ball_index_get_n_normal(b, 3);

	ball_show_state(b);
	print_compare(ba, bc);
	if (ball_weight(b, ba) == ball_weight(b, bc)) {
		ball_update_state(b, ba, NORMAL);
		ans = get_1f1(b, bb);
	}
	else {
		ball_update_state(b, bb, NORMAL);
		if (ball_weight(b, ba) < ball_weight(b, bc)) {
			ball_update_state(b, ba, MAYBE_LIGHT);
			ans = get_1f3_l(b, ba);
		}
		else {
			ball_update_state(b, ba, MAYBE_HEAVY);
			ans = get_1f3_h(b, ba);
		}
	}

	free(ba);
	free(bb);
	free(bc);
	return ans;
}

static int get_1f2(struct balls *b, struct ball_index *bs, struct ball_index *bt)
{
	struct ball_index *bc;
	int ans;
	bc = ball_index_get_n_normal(b, 1);

	ball_show_state(b);
	print_compare(bs, bc);
	if (ball_weight(b, bs) == ball_weight(b, bc)) {
		ball_update_state(b, bs, NORMAL);
		ball_update_state(b, bt, LIGHT);
		ans = bt->a[0];
	}
	else if (ball_weight(b, bs) > ball_weight(b, bc)) {
		ball_update_state(b, bs, HEAVY);
		ball_update_state(b, bt, NORMAL);
		ans = bs->a[0];
	}
	else {
		/*shoud not reach here*/
		fputs("Error!", stderr);
		ans = -1;
	}

	free(bc);
	return ans;
}

static int get_1f8(struct balls *b, struct ball_index *bs, struct ball_index *bt)
{
	/* Now we know that b[bs] > b[bt] */
	struct ball_index *ba1, *ba2, *bb1, *bb2, *bc, *bd, *be;
	int ans;
	ba1 = ball_index_cpart(bs, 0, 1);
	ba2 = ball_index_cpart(bs, 1, 3);
	bb1 = ball_index_cpart(bt, 0, 1);
	bb2 = ball_index_cpart(bt, 1, 3);
	bc = ball_index_get_n_normal(b, 3);
	bd = ball_index_add(ba1, bb2);
	be = ball_index_add(bb1, bc);

	ball_show_state(b);
	print_compare(bd, be);
	if (ball_weight(b, bd) == ball_weight(b, be)) {
		ball_update_state(b, bt, NORMAL);
		ball_update_state(b, ba1, NORMAL);
		ball_update_state(b, ba2, MAYBE_HEAVY);
		ans = get_1f3_h(b, ba2);
	}
	else if (ball_weight(b, bd) > ball_weight(b, be)) {
		ball_update_state(b, ba1, MAYBE_HEAVY);
		ball_update_state(b, ba2, NORMAL);
		ball_update_state(b, bb1, MAYBE_LIGHT);
		ball_update_state(b, bb2, NORMAL);
		ans = get_1f2(b, ba1, bb1);
	}
	else {
		ball_update_state(b, bs, NORMAL);
		ball_update_state(b, bb1, NORMAL);
		ball_update_state(b, bb2, MAYBE_LIGHT);
		ans = get_1f3_l(b, bb2);
	}

	free(ba1);
	free(ba2);
	free(bb1);
	free(bb2);
	free(bc);
	free(bd);
	free(be);
	return ans;
}

static int get_diff(struct balls *b)
{
	int ans;
	struct ball_index *ba, *bb, *bc;
	ba = ball_index_make_ncballs(0, 4);
	bb = ball_index_make_ncballs(4, 4);
	bc = ball_index_make_ncballs(8, 4);
	ball_show_state(b);
	print_compare(ba, bb);
	if (ball_weight(b, ba) == ball_weight(b, bb)) {
		ball_update_state(b, ba, NORMAL);
		ball_update_state(b, bb, NORMAL);
		ans = get_1f4(b, bc);
	}
	else {
		ball_update_state(b, bc, NORMAL);
		if (ball_weight(b, ba) > ball_weight(b, bb)) {
			ans = get_1f8(b, ba, bb);
		}
		else {
			ans = get_1f8(b, bb, ba);
		}
	}
	free(ba);
	free(bb);
	free(bc);
	return ans;
}

int main(void)
{
	struct balls b[12];
	int i, ans;

	for (i = 0; i < 12; ++i) {
		scanf("%d", &b[i].w);
		b[i].state = UNKNOWN;
	}
	ans = get_diff(b);
	ball_show_state(b);
	printf("Index of the different ball is %d, and it's ", ans);
	if (b[ans].state == HEAVY)
		printf("heavier.\n");
	else
		printf("lighter.\n");
	return 0;
}
