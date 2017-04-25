#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "recv.h"

int main(int argc, char *argv[])
{
	char *ifname = NULL;
	struct stat st;
	off_t len, rd;
	void *a;
	struct rec prec;
	int i, fd;

	setlocale(LC_ALL, "");

	i = 1;
	while (i < argc && !strcmp(argv[i], "--"))
		++i;
	if (i < argc)
		ifname = argv[i];

	if (ifname == NULL) {
		g_printerr("Must specify the file name\n");
		exit(1);
	}

	fd = open(ifname, O_RDONLY, 0644);
	fstat(fd, &st);
	if ((st.st_mode & 0170000) != S_IFREG) {
		g_printerr("Input must be a regular file\n");
		exit(1);
	}
	len = st.st_size;
	rd = 0;

	a = mmap(NULL, len, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
	if (a == MAP_FAILED) {
		perror("mmap failed:");
		exit(1);
	}
	

	while (rd < len) {
		rd += read_rec(a + rd, &prec);
		write_line(1, &prec);
	}

	return 0;
}
