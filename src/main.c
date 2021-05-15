#include "ft_ping.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
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
			arg++;
			if (*arg == 'c') {
				arg++;
				if (*arg == 0) {
					if (i < argc) {
						arg = argv[i++];
					}
					else {
						err_fmt(INVALID_COUNT_OF_PACKETS, "invalid count of packets to transmit: '%s'", arg);
					}
				}
				if (ft_safe_atoi(arg, &ft_ping.count) != FT_ATOI_OK || ft_ping.count <= 0) {
					err_fmt(INVALID_COUNT_OF_PACKETS, "invalid count of packets to transmit: '%s'", arg);
				}
				dlog("count of packets: %d", ft_ping.count);
			}
		}
		else
		{
			if (ft_ping.host == NULL) {
				ft_ping.host = arg;
			}
			else
				return UNKNOWN_HOST;
		}
	}
	return OK;
}

void send_req() {
	int sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0) {
		dlog("socket error: %d, errno: %d", sock, errno);
	}
	alarm(1);
}

void alrm_handler(int sig) {
	dlog("hi: %d", sig);
	if (ft_ping.count > 0) {
		ft_ping.count--;
	}
	if (ft_ping.count != 0) {
		send_req();
	}
	else {
		exit(0);
	}
}

void int_handler(int sig) {
	dlog("exit: %d", sig);
	exit(0);
}

const char *ntop(struct sockaddr *addr, char *dst, size_t size) {
	return inet_ntop(addr->sa_family, addr, dst, size);
}

int sizeaddr(struct addrinfo *res) {
	int size = 0;
	while (res) {
		size++;
		struct sockaddr *addr = res->ai_addr;
		struct sockaddr_in *addr_in = addr;
		dlog("len: %d, family: %d, addr: %s", addr->sa_len, addr->sa_family, inet_ntoa(addr_in->sin_addr));
		res = res->ai_next;
	}
	return size;
}

int f(char *addr){
	struct addrinfo hints;
	struct addrinfo *result;
	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	int t = getaddrinfo( addr, NULL, &hints, &result);
	if (t == 0) {
		ft_ping.sin_addr = ((struct sockaddr_in *)result->ai_addr)->sin_addr;
	}
	return t;
}

int main(int argc, char *argv[]) {
	ft_memset(&ft_ping, 0, sizeof(ft_ping));
	ft_ping.delay = 1;
	ft_ping.count = -1;
	parse_argv(argc - 1, argv + 1);
	if (signal(SIGALRM, alrm_handler) == SIG_ERR) {
		err(CANNOT_SET_SIGNAL);
	}
	if (signal(SIGINT, int_handler) == SIG_ERR) {
		err(CANNOT_SET_SIGNAL);
	}
	if (f(ft_ping.host) != 0) {
		err_fmt(UNKNOWN_HOST, "unknown host: %s", ft_ping.host);
	}
	inet_ntop(AF_INET, &ft_ping.sin_addr, ft_ping.addr, sizeof(ft_ping.addr) -1);
	dlog("host: %s", ft_ping.addr);
	dlog("hello");
	send_req();
	while(42) {
	}
}
