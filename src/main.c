#include "ft_ping.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

t_ft_ping ft_ping;

void fill_payload(unsigned char *payload, int size) {
	int i;
	int alphabet_size;

	i = 0;
	alphabet_size = 'z' - 'a' + 1;
	while (i < size)
	{
		*payload++ = 'a' + i % alphabet_size;
		i++;
	}
}

void send_icmp()
{
	struct icmp *icmp;
	unsigned char *payload;

	icmp = (struct icmp *)ft_ping.snd_buf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = ft_ping.pid;
	icmp->icmp_seq = ++ft_ping.seq;
	icmp->icmp_cksum = 0;
	payload = ft_ping.snd_buf + 8;
	fill_payload(payload, ft_ping.packet_size);
	icmp->icmp_cksum = chksum(ft_ping.snd_buf, ft_ping.packet_size + 8);
	size_t res = sendto(ft_ping.sock, ft_ping.snd_buf, ft_ping.packet_size + 8,
						  0, ft_ping.ai_addr, ft_ping.ai_addrlen);
	dlog("sent: %lu", res);
	alarm(1);
}

void prepare_socket()
{
	int ttl_val;
	struct timeval tv_out;

	ft_ping.sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ft_ping.sock < 0) {
		dlog("socket error: %d, errno: %s", ft_ping.sock, strerror(errno));
	}
	ttl_val = 64;
	if (setsockopt(ft_ping.sock, SOL_IP, IP_TTL,
				   &ttl_val, sizeof(ttl_val)) != 0)
	{
		dlog("setsockopt() for IP_TTL error %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	tv_out.tv_sec = 30;
	tv_out.tv_usec = 0;
	if (setsockopt(ft_ping.sock, SOL_SOCKET, SO_RCVTIMEO,
			   (const char*)&tv_out, sizeof tv_out) != 0) {
		dlog("setsockopt() for SO_RCVTIMEO error %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void alrm_handler(int sig) {
	dlog("hi: sig: %d, count: %d", sig, ft_ping.count);
	if (ft_ping.count > 0)
		ft_ping.count--;
	if (ft_ping.count != 0)
		send_icmp();
}

void int_handler(int sig) {
	dlog("exit: %d", sig);
	exit(0);
}

const char *ntop(struct sockaddr *addr, char *dst, size_t size) {
	return inet_ntop(addr->sa_family, addr, dst, size);
}

int get_addr(char *addr)
{
	struct addrinfo *result;

	int t = getaddrinfo(addr, NULL, NULL, &result);
	if (t == 0)
	{
		ft_ping.ai_addr = result->ai_addr;
		ft_ping.ai_addrlen = result->ai_addrlen;
		ft_ping.sin_addr = ((struct sockaddr_in *)result->ai_addr)->sin_addr;
	}
	return t;
}

void init_ft_ping()
{
	ft_memset(&ft_ping, 0, sizeof(ft_ping));
	ft_ping.delay = 1;

	ft_ping.pid = getpid();
	ft_ping.seq = 0;
	ft_ping.packet_size = 56;
	ft_ping.sock = -1;
	ft_ping.count = -1;
	ft_ping.count_total = -1;
	ft_ping.count_managed = 0;
}

int main(int argc, char *argv[])
{
	init_ft_ping();
	parse_argv(argc - 1, argv + 1);
	if (ft_ping.host == NULL || get_addr(ft_ping.host) != 0) {
		err_fmt(UNKNOWN_HOST, "unknown host: %s", ft_ping.host);
	}
	if (signal(SIGALRM, alrm_handler) == SIG_ERR) {
		err_fmt(CANNOT_SET_SIGNAL, "cannot set signal: %d", SIGALRM);
	}
	if (signal(SIGINT, int_handler) == SIG_ERR) {
		err_fmt(CANNOT_SET_SIGNAL, "cannot set signal: %d", SIGINT);
	}
	inet_ntop(AF_INET, &ft_ping.sin_addr, ft_ping.addr, sizeof(ft_ping.addr) -1);
	dlog("host: %s", ft_ping.addr);
	prepare_socket();
	send_icmp();
	while (ft_ping.count_managed != ft_ping.count_total)
	{
		unsigned char recv_buf[65535];
		unsigned char ancdata[65535];
		if (ft_ping.sock != -1)
		{
			struct iovec io;
			struct msghdr msg;
			struct sockaddr_in recv;

			ft_memset(&msg, 0, sizeof(msg));
			ft_memset(&recv, 0, sizeof(recv));
			io.iov_base = recv_buf;
			io.iov_len = sizeof(recv_buf);
			msg.msg_iov = &io;
			msg.msg_iovlen = 1;
			msg.msg_control = ancdata;
			msg.msg_controllen = sizeof(ancdata);
			msg.msg_flags = 0;
			msg.msg_name = &recv;
			msg.msg_namelen = sizeof(recv);
			ssize_t received = recvmsg(ft_ping.sock, &msg, 0);
			if (received < 0) {
				if (errno != EINTR)
				{
					dlog("recvmsg error: %d, %s", errno, strerror(errno));
				}
			} else {
				dlog("received %ld bytes", received);
			}
		}
	}
}
