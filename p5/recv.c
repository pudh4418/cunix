#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "recv.h"

size_t read_rec(void *offset, struct rec *p)
{
	guint64 x;
	size_t l;
	guint16 y;

	x = 0;
	x = *(guint64 *)offset;
	p->id = GINT64_FROM_BE(x);
	offset += sizeof(x);

	y = *(guint16 *)offset;
	p->name_len = GINT16_FROM_BE(y);
	offset += sizeof(y);

	p->d_len = *(guint8 *)offset;
	offset += 1;
	p->age = *(guint8 *)offset;
	offset += 1;

	p->name = malloc(p->name_len);
	strcpy(p->name, (const char *)offset);
	offset += p->name_len;

	p->d = malloc(p->d_len);
	strcpy(p->d, (const char *)offset);
	offset += p->d_len;

	l = 12 + p->name_len + p->d_len;
	if (l % 8) {
		offset += 8 - l%8;
		l += 8 - l%8;
	}
	return l;
}

void write_line(int fd, struct rec *p)
{
	char *s;
	s =  g_strdup_printf( "%" G_GUINT64_FORMAT ",%s,%s,%d\n", p->id, p->name, p->d, p->age);
	write(fd, s, strlen(s));
}

