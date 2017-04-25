#ifndef RECV_H
#define RECV_H

#include <glib.h>
#include "rec.h"

size_t read_rec(void *offset, struct rec *p);
void write_line(int fd, struct rec *p);
#endif
