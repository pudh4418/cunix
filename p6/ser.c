#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <glib.h>

#define P_PORT "23333"

struct cli_node {
	int sock;
	socklen_t len;
	struct sockaddr_storage sa;
};
#define cli_sock(x) (((struct cli_node *) (x->data))->sock)
#define cli_ss(x) (((struct cli_node *) (x->data))->sa)

static const char *cause[] = {NULL, "socket", "bind", "listen"};

static void set_fd(gpointer data, gpointer ud)
{
	int x = GPOINTER_TO_INT(data);
	fd_set *fds = (fd_set *) ud;
	FD_SET(x, fds);
}	

static void print_log(struct sockaddr_storage *ss, int stat)
{
	char buf[INET6_ADDRSTRLEN];
	const char *str;
	static const char *v5[2] = {"Connected", "Disconnected"};
	if (ss->ss_family == AF_INET) {
		str = inet_ntop(AF_INET, &((struct sockaddr_in *) ss)->sin_addr, buf, INET6_ADDRSTRLEN);
		fprintf(stderr, "[LOG] %s from %s, port %hu.\n", v5[stat], str, ntohs(((struct sockaddr_in *) ss)->sin_port));
	}
	else if (ss->ss_family == AF_INET6) {
		str = inet_ntop(AF_INET6, &((struct sockaddr_in6 *) ss)->sin6_addr, buf, 4096);
		fprintf(stderr, "[LOG] %s from %s, port %hu.\n", v5[stat], str, ntohs(((struct sockaddr_in6 *) ss)->sin6_port));
	}
}

int main(void)
{
	struct addrinfo hints, *res, *res0;
	int error, maxn, st;
	fd_set fds, afds;
	GList *s, *l, *cli, *k;
	char buf[4096];
	ssize_t nread;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
	error = getaddrinfo(NULL, P_PORT, &hints, &res0);
	if (error) {
		fprintf(stderr, "%s\n", gai_strerror(error));
		return 1;
	}

	s = NULL;
	for (res = res0; res; res = res->ai_next) {
		st = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (st < 0) {
			error = 1;
			continue;
		}
		if (bind(st, res->ai_addr, res->ai_addrlen) < 0) {
			error = 2;
			close(st);
			continue;
		}
		if (listen(st, 5) < 0) {
			error = 3;
			close(st);
			continue;
		}
		s = g_list_prepend(s, GINT_TO_POINTER(st));
	}
	freeaddrinfo(res0);
	if (s == NULL) {
		fprintf(stderr, "%s failed.\n", cause[error]);
		return 1;
	}

	FD_ZERO(&afds);
	g_list_foreach(s, set_fd, &afds);
	maxn = -1;
	for (l = s; l != NULL; l = l->next)
		if (GPOINTER_TO_INT(l->data) > maxn)
			maxn = GPOINTER_TO_INT(l->data);
	maxn += 1;
	cli = NULL;
	fds = afds;

	while (select(maxn, &fds, NULL, NULL, NULL) > 0) {
		for (l = s; l != NULL; l = l->next)
			if (FD_ISSET(GPOINTER_TO_INT(l->data), &fds)) {
				int x;
				struct cli_node *cn;
				struct sockaddr_storage sa;
				socklen_t size = sizeof(sa);

				x = accept(GPOINTER_TO_INT(l->data),
					   (struct sockaddr *) &sa, &size);
				if (x < 0) {
					fprintf(stderr, "accept failed.\n");
					return 1;
				}
				FD_SET(x, &afds);
				cn = g_new(struct cli_node, 1);
				cn->sock = x;
				cn->len = size;
				cn->sa = sa;
				cli = g_list_prepend(cli, cn);
				print_log(&sa, 0);
			}
		l = cli;
		while (l != NULL) {
			GList *ne = l->next;
			if (FD_ISSET(cli_sock(l), &fds)) {
				int x = cli_sock(l);
				nread = read(x, buf, 4096);
				if (nread == 0) {
					print_log(&cli_ss(l), 1);
					close(x);
					FD_CLR(x, &afds);
					cli = g_list_remove_link(cli, l);
					g_list_free_full(l, (GDestroyNotify) g_free);
				}
				else {
					for (k = cli; k != NULL; k = k->next)
						if (cli_sock(k) != x)
							write(cli_sock(k), buf, nread);
					write(1, buf, nread);
				}
			}
			l = ne;
		}

		maxn = -1;
		for (l = s; l != NULL; l = l->next)
			if (GPOINTER_TO_INT(l->data) > maxn)
				maxn = GPOINTER_TO_INT(l->data);
		for (l = cli; l != NULL; l = l->next)
			if (((struct cli_node *) (l->data))->sock > maxn)
				maxn = ((struct cli_node *) (l->data))->sock;
		maxn += 1;
		fds = afds;
	}
	return 0;
}
