#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>

static const char *cause[] = {NULL, "socket", "connect"};

int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *res0;
	int error, s, maxn;
	fd_set fds;
	char buf[4096], *str;
	ssize_t nread;

	if (argc != 4) {
		fprintf(stderr, "Usage: %s SERV_ADDR SERV_PORT NICKNAME\n", argv[0]);
		return 1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_NUMERICSERV;
	error = getaddrinfo(argv[1], argv[2], &hints, &res0);
	if (error) {
		fprintf(stderr, "%s\n", gai_strerror(error));
		return 1;
	}

	s = -1;
	for (res = res0; res; res = res->ai_next) {
		s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (s < 0) {
			error = 1;
			continue;
		}
		if (connect(s, res->ai_addr, res->ai_addrlen) < 0) {
			error = 2;
			close(s);
			s = -1;
			continue;
		}
		break;
	}
	freeaddrinfo(res0);
	if (s < 0) {
		fprintf(stderr, "%s failed.\n", cause[error]);
		return 1;
	}

	if (s > 1)
		maxn = s + 1;
	else
		maxn = 2;
	
	FD_ZERO(&fds);
	FD_SET(s, &fds);
	FD_SET(1, &fds);
	while (select(maxn, &fds, NULL, NULL, NULL) > 0) {
		if (FD_ISSET(s, &fds)) {
			nread = read(s, buf, 4096);
			write(1, buf, nread);
		}
		if (FD_ISSET(1, &fds)) {
			nread = read(1, buf, 4096);
			buf[nread] = '\0';
			asprintf(&str, "[%s] %s", argv[3], buf);
			write(s, str, strlen(str));
			free(str);
		}
		FD_ZERO(&fds);
		FD_SET(s, &fds);
		FD_SET(1, &fds);
	}
	return 0;
}
