#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "pspt.h"

#define INPUT_FILE "data.txt"

void pspt(char *ofn)
{
	int i, j;
	pid_t pids[11];
	FILE *fin, *fout;
	char *sn, *line;
	size_t n = 0;
	ssize_t read;
	char buf[4096];

	fin = fopen(INPUT_FILE, "rb");
	line = NULL;
	for (i=1; i<=10; ++i) {
		sn = g_strdup_printf("s%d.txt", i);
		fout = fopen(sn, "wb");
		g_free(sn);
		for (j=0; j<100; ++j) {
			read = getline(&line, &n, fin);
			fwrite(line, 1, read, fout);
		}
		fclose(fout);
	}
	free(line);
	fclose(fin);

	char *argv[5] = {"./t4", "-o", NULL, NULL, NULL};
	for (i=1; i<=10; ++i) {
		argv[2] = g_strdup_printf("o%d.dat", i);
		argv[3] = g_strdup_printf("s%d.txt", i);
		if ((pids[i] = fork()) == 0)
			execvp(argv[0], argv);
		g_free(argv[2]);
		g_free(argv[3]);
	}
	for (i=1; i<=10; ++i)
		wait(NULL);

	fout = ofn ? fopen(ofn, "wb") : stdout;
	for (i=1; i<=10; ++i) {
		sn = g_strdup_printf("o%d.dat", i);
		fin = fopen(sn, "rb");
		g_free(sn);
		while ((n = fread(buf, 1, 4096, fin)) != 0) {
			n = fwrite(buf, 1, n, fout);
		}
		fclose(fin);
	}
	fclose(fout);
}
