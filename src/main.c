#include "ft_ping.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	unsigned char snd_buf[SEND_BUF_SZ];
	struct icmp *icmp;
	unsigned char *payload;

	icmp = (struct icmp *)snd_buf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = ft_ping.pid;
	icmp->icmp_seq = ++ft_ping.seq;
	icmp->icmp_cksum = 0;
	payload = snd_buf + 8;
	fill_payload(payload, ft_ping.packet_size);
	icmp->icmp_cksum = chksum(snd_buf, ft_ping.packet_size + 8);
	ssize_t res = sendto(ft_ping.sock, snd_buf, ft_ping.packet_size + 8,
						  0, &ft_ping.ai_addr, ft_ping.ai_addrlen);
	if (res >= 0) {
		ft_ping.packets_send++;
		add_to_queue(&ft_ping.root, ft_ping.seq);
	}
	dlog("sent: %ld", res);
	alarm(1);
}

void prepare_socket()
{
	ft_ping.sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ft_ping.sock < 0) {
		dlog("socket error: %d, errno: %s", ft_ping.sock, strerror(errno));
	}
	if (setsockopt(ft_ping.sock, SOL_IP, IP_TTL,
				   &ft_ping.ttl, sizeof(ft_ping.ttl)) != 0)
	{
		dlog("setsockopt() for IP_TTL error %s", strerror(errno));
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

int get_addr(char *addr)
{
	struct addrinfo *result;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	int t = getaddrinfo(addr, NULL, &hints, &result);
	if (t == 0)
	{
		ft_memcpy(&ft_ping.ai_addr, result->ai_addr, sizeof(struct sockaddr));
		ft_ping.ai_addrlen = result->ai_addrlen;
		ft_ping.sin_addr = ((struct sockaddr_in *)&ft_ping.ai_addr)->sin_addr;
		freeaddrinfo(result);
	}
	return t;
}

void init_ft_ping()
{
	ft_memset(&ft_ping, 0, sizeof(ft_ping));
	ft_ping.delay = 1;

	ft_ping.ttl = 54;
	ft_ping.pid = getpid();
	ft_ping.seq = 0;
	ft_ping.packet_size = 56;
	ft_ping.sock = -1;
	ft_ping.count = -1;
	ft_ping.count_total = -1;
}

long tv_diff(struct timeval *tv1, struct timeval *tv2) {
	return (tv1->tv_sec - tv2->tv_sec) * 1000000 + (tv1->tv_usec - tv2->tv_usec);
}

void parse_ipv4(const unsigned char *recv_buf, ssize_t received)
{
	dlog("received %ld bytes", received);
	struct ip * ip = (struct ip *)recv_buf;
	dlog("sizeof(ip): %d, protocol: %d, tos: %d, len: %d, id: %d, off: %d",
		 sizeof(struct ip), ip->ip_p, ip->ip_tos, ntohs(ip->ip_len), ntohs(ip->ip_id), ntohs(ip->ip_off));
	if (ip->ip_p == IPPROTO_ICMP) {
		struct icmp *icmp = (struct icmp *)(recv_buf + sizeof(struct ip));
		if (icmp->icmp_type == ICMP_ECHOREPLY && icmp->icmp_hun.ih_idseq.icd_id == ft_ping.pid ) {
			dlog("type: %d, id: %d, seq: %d", icmp->icmp_type, icmp->icmp_hun.ih_idseq.icd_id, icmp->icmp_hun.ih_idseq.icd_seq);
			t_ping_elem *elem = poll_elem(&ft_ping.root, icmp->icmp_hun.ih_idseq.icd_seq);
			if (elem != NULL) {
				struct timeval tv;
				gettimeofday(&tv, NULL);
				long diff = tv_diff(&tv, &elem->send_time);

				char text_ip[64];
				inet_ntop(AF_INET, &ip->ip_src, text_ip, sizeof(text_ip) - 1);

				dprintf(1, "%lu bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms\n",
					 received - sizeof(struct ip), ft_ping.host, text_ip,
						icmp->icmp_hun.ih_idseq.icd_seq, ip->ip_ttl, diff/1000.0);
				ft_ping.packets_recv++;
				ft_memdel((void **)&elem);
			}
		}
	}
}

static void recv_msg()
{
	struct iovec io;
	struct msghdr msg;
	struct sockaddr_in recv;
	unsigned char recv_buf[RECV_BUF_SZ];

	ft_memset(&msg, 0, sizeof(msg));
	ft_memset(&recv, 0, sizeof(recv));
	io.iov_base = recv_buf;
	io.iov_len = sizeof(recv_buf);
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
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
		parse_ipv4(recv_buf, received);
	}
}

int main(int argc, char *argv[])
{
	init_ft_ping();
	parse_argv(argc - 1, argv + 1);
	if (ft_ping.host == NULL) {
		print_usage(2);
	}
	if (get_addr(ft_ping.host) != 0) {
		dprintf(2, "ping: %s: Name or service not known\n", ft_ping.host);
	}
	if (signal(SIGALRM, alrm_handler) == SIG_ERR) {
		err_fmt(CANNOT_SET_SIGNAL, "cannot set signal: %d", SIGALRM);
	}
	if (signal(SIGINT, int_handler) == SIG_ERR) {
		err_fmt(CANNOT_SET_SIGNAL, "cannot set signal: %d", SIGINT);
	}
	inet_ntop(AF_INET, &ft_ping.sin_addr, ft_ping.addr, sizeof(ft_ping.addr) -1);
	dprintf(1, "PING %s (%s) %d(%lu) bytes of data.\n", ft_ping.host,
			ft_ping.addr, ft_ping.packet_size,
			ft_ping.packet_size + 8 + sizeof(struct ip));
	dlog("host: %s", ft_ping.addr);
	prepare_socket();
	send_icmp();
	while ((ft_ping.packets_recv + ft_ping.packets_loss) != ft_ping.count_total)
	{
		if (ft_ping.sock != -1)
		{
			recv_msg();
		}
	}
}

