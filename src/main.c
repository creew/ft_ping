#include "ft_ping.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

static t_ft_ping ft_ping;

int parse_arg(char *arg, int *narg, int argc) {
	return 0;
}

int parseOptionC(int argc, char *const *argv, char *arg, int i)
{
	int err;

	err = 0;
	arg++;
	if (*arg == 0) {
		if (i < argc) {
			arg = argv[i++];
		}
		else {
			err = 1;
		}
	}
	if (err || ft_safe_atoi(arg, &ft_ping.count) != FT_ATOI_OK || ft_ping.count <= 0) {
		err_fmt(INVALID_COUNT_OF_PACKETS, "invalid count of packets to transmit: '%s'", arg);
	}
	dlog("count of packets: %d", ft_ping.count);
	return i;
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
				i = parseOptionC(argc, argv, arg, i);
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
	int on;
	char send_buf[400], recv_buf[400], src_name[256], src_ip[15], dst_ip[15];
	struct ip *ip = (struct ip *)send_buf;
	struct icmp *icmp = (struct icmp *)(ip + 1);
	struct sockaddr_in src, dst;
	int sock;

	sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0) {
		dlog("socket error: %d, errno: %s", sock, strerror(errno));
	}
	on = 1;
	if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
	{
		dlog("setsockopt() for IP_HDRINCL error %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* IP structure, check the ip.h */
	ip->ip_v = 4;
	ip->ip_hl = 5;
	ip->ip_tos = 0;
	ip->ip_len = htons(sizeof(send_buf));
	ip->ip_id = htons(321);
	ip->ip_off = htons(0);
	ip->ip_ttl = 255;
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_sum = 0;

	/* ICMP structure, check ip_icmp.h */
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = 123;
	icmp->icmp_seq = 0;
	icmp->icmp_cksum = 0;

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
		dlog("family: %d, addr: %s", addr->sa_family, inet_ntoa(addr_in->sin_addr));
		res = res->ai_next;
	}
	return size;
}

int f(char *addr){
	struct addrinfo *result;
	int t = getaddrinfo(addr, NULL, NULL, &result);
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
