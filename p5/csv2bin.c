#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <fcntl.h>
#include <glib.h>

#define DICT_NUM 4

#include "rec.h"

static GOptionEntry entries[] =
{
	{NULL, '\0', 0, 0, NULL, NULL, NULL}
};

int main(int argc, char *argv[])
{
	GError *err = NULL;
	GOptionContext *context;
	char *ifname = NULL;
	int i;
	int in_fd, out_fd, din_fd[DICT_NUM][2], dout_fd[DICT_NUM][2];
	int t[DICT_NUM];

	setlocale(LC_ALL, "");

	context = g_option_context_new(" - convert student record");
	g_option_context_add_main_entries(context, entries, NULL);
	if (!g_option_context_parse(context, &argc, &argv, &err)) {
		g_print("Failed to parse option: %s\n", err->message);
		g_option_context_free(context);
		exit(1);
	}
	g_option_context_free(context);

	i = 1;
	while (i < argc && !strcmp(argv[i], "--"))
		++i;
	if (i < argc)
		ifname = argv[i];

	in_fd = ifname ? open(ifname, O_RDONLY) : STDIN_FILENO;
	for (i = 0; i < DICT_NUM; ++i)
		pipe(din_fd[i]);
	if (fork() == 0) {
		for (i = 0; i < DICT_NUM; ++i) {
			t[i] = din_fd[i][1];
			close(din_fd[i][0]);
		}
		disp(in_fd, t, DICT_NUM);
		exit(0);
	}
	close(in_fd);
	for (i = 0; i < DICT_NUM; ++i)
		close(din_fd[i][1]);

	for (i = 0; i < DICT_NUM; ++i)
		pipe(dout_fd[i]);
	for (i = 0; i < DICT_NUM; ++i) {
		if (fork() == 0) {
			close(dout_fd[i][0]);
			conv(din_fd[i][0], dout_fd[i][1]);
			exit(0);
		}
		close(dout_fd[i][1]);
	}

	out_fd = STDOUT_FILENO;
	if (fork() == 0) {
		for (i = 0; i < DICT_NUM; ++i) {
			t[i] = dout_fd[i][0];
			close(dout_fd[i][1]);
		}
		merge(out_fd, t, DICT_NUM);
		exit(0);
	}
	close(out_fd);
	for (i = 0; i < DICT_NUM; ++i)
		close(dout_fd[i][0]);

	for (i = 0; i < DICT_NUM + 2; ++i)
		wait(NULL);
	return 0;
}
