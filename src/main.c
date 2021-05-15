#include "ft_ping.h"

#include <arpa/inet.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static t_ft_ping ft_ping;

int parse_arg(char *arg, int *narg, int argc) {
	return 0;
}

int parse_argv(int argc, char *argv[])
{
	char *arg;
	int i;
	i = 0;
	while (i < argc)
	{
		arg = argv[i++];
		if (*arg == '-')
		{

		}
		else
		{
			if (ft_ping.host != NULL) {
				ft_ping.host = arg;
			}
			else
				return UNKNOWN_HOST;
		}
		printf("%s\n", arg);
	}
	return OK;
}

void err(int err) {
	exit(err);
}

void alrm_handler(int sig) {
	dlog("hi: %d", sig);
}

void send_req() {
	alarm(1);
}

int main(int argc, char *argv[]) {
	ft_memset(&ft_ping, 0, sizeof(ft_ping));
	ft_ping.delay = 1;
	parse_argv(argc - 1, argv + 1);
	if (signal(SIGALRM, alrm_handler) == SIG_ERR) {
		err(CANNOT_SET_SIGNAL);
	}
	dlog("hello");
	send_req();
	while(42) {

	}
	//print_usage();
	return 0;
}
