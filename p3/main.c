#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <glib.h>
#include "rec.h"

static gboolean disp = FALSE;
static gchar *ofname = NULL;

static GOptionEntry entries[] =
{
	{"print", 'p', G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE, &disp, "Print the record", NULL},
	{"output", 'o', G_OPTION_FLAG_NONE, G_OPTION_ARG_FILENAME, &ofname, "Write optput to file", "FILE"},
	{NULL, '\0', 0, 0, NULL, NULL, NULL}
};

int main(int argc, char *argv[])
{
	GError *err = NULL;
	GOptionContext *context;
	char *ifname = NULL;
	int i;

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
	if (disp)
		btot(ifname, ofname);
	else
		ttob(ifname, ofname);
	return 0;
}
