#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include "rec.h"

static gchar *get_field(FILE *fin, char d)
{
	char *field = NULL;
	size_t n = 0;
	ssize_t read;
	
	read = getdelim(&field, &n, d, fin);
	if (read == -1)
		return NULL;
	field[read - 1] = '\0';
	return field;
}

static gboolean read_line(FILE *fin, struct rec *r)
{
	char *s;
	size_t l;
	guint64 t;
	s = get_field(fin, ',');
	if (s == NULL)
		return FALSE;
	r->id = g_ascii_strtoull(s, NULL, 10);
	free(s);

	s = get_field(fin, ',');
	if (s == NULL)
		return FALSE;
	r->name = strdup(s);
	l = strlen(s) + 1;
	if (l > G_MAXUINT16) {
		g_printerr("Name is too long...\n");
		exit(1);
	}
	r->name_len = (guint16) l;
	free(s);

	s = get_field(fin, ',');
	if (s == NULL)
		return FALSE;
	r->d = strdup(s);
	l = strlen(s) + 1;
	if (l > G_MAXUINT8) {
		g_printerr("Department is too long...\n");
		exit(1);
	}
	r->d_len = (guint8) l;
	free(s);

	s = get_field(fin, '\n');
	if (s == NULL)
		return FALSE;
	t = g_ascii_strtoull(s, NULL, 10);
	if (t > G_MAXUINT8) {
		g_printerr("s = %s, t = %lu Excited!\n", s, t);
		exit(1);
	}
	r->age = (guint8) t;
	free(s);
	return TRUE;
}

static void write_rec(int fout, struct rec *p)
{
	guint64 x;
	size_t l;
	guint16 y;
	guint8 z;

	x = GINT64_TO_BE(p->id);
	write(fout, &x, sizeof(x));
	y = GINT16_TO_BE(p->name_len);
	write(fout, &y, sizeof(y));
	z = p->d_len;
	write(fout, &z, sizeof(z));
	z = p->age;
	write(fout, &z, sizeof(z));
	write(fout, p->name, p->name_len);
	write(fout, p->d, p->d_len);
	free(p->name);
	free(p->d);

	l = 12 + p->name_len + p->d_len;
	if (l % 8) {
		x = 0;
		write(fout, &x, 8 - l%8);
	}
	
}

void disp(int i_fd, int *o_fd, int n)
{
	char *s = NULL;
	FILE *fin = fdopen(i_fd, "rb");
	ssize_t nread;
	size_t cap = 0;
	int i = 0;

	while ((nread = getline(&s, &cap, fin)) != -1) {
		++i;
		write(o_fd[i % n], s, nread);
	}
}

void conv(int i_fd, int o_fd)
{
	FILE *fin = fdopen(i_fd, "rb");
	struct rec prec;

	while (read_line(fin, &prec)) {
		write_rec(o_fd, &prec);
	}
}

void merge(int o_fd, int *i_fd, int n)
{
	char buf[1024];
	ssize_t nread;
	int i;

	for (i = 0; i < n; ++i) {
		while ((nread = read(i_fd[i], buf, 1024)) != 0)
			write(o_fd, buf, nread);
	}
}
