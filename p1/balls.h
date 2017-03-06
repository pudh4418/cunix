#ifndef BALLS_H
#define BALLS_H

struct ball_index {
	int n;
	int a[12];
};

struct balls {
	int w;
	enum ball_state {
		UNKNOWN,
		NORMAL,
		MAYBE_HEAVY,
		MAYBE_LIGHT,
		HEAVY,
		LIGHT
	} state;
};

struct ball_index *ball_index_cpart(struct ball_index *bi, int start, int l);
struct ball_index *ball_index_get_n_normal(struct balls *b, int n);
struct ball_index *ball_index_make_ncballs(int x, int n);
struct ball_index *ball_index_add(struct ball_index *bi, struct ball_index *bx);
int ball_weight(struct balls *b, struct ball_index *bi);
void ball_update_state(struct balls *b, struct ball_index *bi, enum ball_state state);
void ball_show_state(struct balls *b);

#endif

