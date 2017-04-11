#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
		g_print("Name is too long...\n");
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
		g_print("Department is too long...\n");
		exit(1);
	}
	r->d_len = (guint8) l;
	free(s);

	s = get_field(fin, '\n');
	if (s == NULL)
		return FALSE;
	t = g_ascii_strtoull(s, NULL, 10);
	if (t > G_MAXUINT8) {
		g_print("Excited!\n");
		exit(1);
	}
	r->age = (guint8) t;
	free(s);
	return TRUE;
}

static void write_rec(FILE *fout, struct rec *p)
{
	guint64 x;
	size_t l;
	guint16 y;
	guint8 z;

	x = GINT64_TO_BE(p->id);
	fwrite(&x, sizeof(x), 1, fout);
	y = GINT16_TO_BE(p->name_len);
	fwrite(&y, sizeof(y), 1, fout);
	z = p->d_len;
	fwrite(&z, sizeof(z), 1, fout);
	z = p->age;
	fwrite(&z, sizeof(z), 1, fout);
	fwrite(p->name, p->name_len, 1, fout);
	fwrite(p->d, p->d_len, 1, fout);
	free(p->name);
	free(p->d);

	l = 12 + p->name_len + p->d_len;
	if (l % 8) {
		x = 0;
		fwrite(&x, 8 - l%8, 1, fout);
	}
	
}

static gboolean read_rec(FILE *fin, struct rec *p)
{
	guint64 x;
	size_t l;
	guint16 y;

	x = 0;
	l = fread(&x, 1, sizeof(x), fin);
	if (l != sizeof(x))
		return FALSE;
	p->id = GINT64_FROM_BE(x);

	l = fread(&y, 1, sizeof(y), fin);
	if (l != sizeof(y))
		return FALSE;
	p->name_len = GINT16_FROM_BE(y);

	l = fread(&p->d_len, 1, 1, fin);
	if (l != 1)
		return FALSE;
	l = fread(&p->age, 1, 1, fin);
	if (l != 1)
		return FALSE;

	p->name = malloc(p->name_len);
	l = fread(p->name, 1, p->name_len, fin);
	if (l != p->name_len)
		return FALSE;

	p->d = malloc(p->d_len);
	l = fread(p->d, 1, p->d_len, fin);
	if (l != p->d_len)
		return FALSE;

	l = 12 + p->name_len + p->d_len;
	if (l % 8)
		fread(&x, 8 - l%8, 1, fin);
	return TRUE;
}

static void write_line(FILE *fout, struct rec *p)
{
	fprintf(fout, "%" G_GUINT64_FORMAT ",%s,%s,%d\n", p->id, p->name, p->d, p->age);
}

void btot(char *ifn, char *ofn)
{
	FILE *fin, *fout;
	struct rec prec;

	fin = ifn ? fopen(ifn, "rb") : stdin;
	fout = ofn ? fopen(ofn, "wb") : stdout;
	while (read_rec(fin, &prec)) {
		write_line(fout, &prec);
	}
	
}

void ttob(char *ifn, char *ofn)
{
	FILE *fin, *fout;
	struct rec prec;

	fin = ifn ? fopen(ifn, "rb") : stdin;
	fout = ofn ? fopen(ofn, "wb") : stdout;
	while (read_line(fin, &prec)) {
		write_rec(fout, &prec);
	}
}

