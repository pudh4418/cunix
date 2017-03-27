#ifndef REC_H
#define REC_H

#include <glib.h>

struct rec_head {
	guchar magic[6];
	guint16 ver;
	guint64 num;
};

struct rec {
	guint64 id;
	gchar *name;
	gchar *d;
	guint16 name_len;
	guint8 d_len;
	guint8 age;
};

void btot(char *, char *);
void ttob(char *, char *);
#endif
