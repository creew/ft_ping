/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 21:54:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/21 21:54:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	send_icmp(void)
{
	unsigned char	snd_buf[SEND_BUF_SZ];
	struct icmp		*icmp;
	ssize_t			res;

	icmp = (struct icmp *)snd_buf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = g_ft_ping.pid;
	icmp->icmp_seq = ++g_ft_ping.seq;
	icmp->icmp_cksum = 0;
	fill_payload(snd_buf + ICMP_PACKET_SZ, g_ft_ping.packet_size);
	icmp->icmp_cksum = chksum(snd_buf, g_ft_ping.packet_size + ICMP_PACKET_SZ);
	res = sendto(g_ft_ping.sock, snd_buf,
			g_ft_ping.packet_size + ICMP_PACKET_SZ,
			0, &g_ft_ping.ai_addr, g_ft_ping.ai_addrlen);
	if (res >= 0)
	{
		g_ft_ping.packets_send++;
		add_to_queue(&g_ft_ping.root, g_ft_ping.seq);
	}
	dlog("sent: %ld", res);
	alarm(g_ft_ping.interval);
}

void	prepare_socket(void)
{
	g_ft_ping.sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_ft_ping.sock < 0)
		dlog("socket error: %d, errno: %s", g_ft_ping.sock, strerror(errno));
	if (setsockopt(g_ft_ping.sock, SOL_IP, IP_TTL,
			&g_ft_ping.ttl, sizeof(g_ft_ping.ttl)) != 0)
	{
		err_fmt(CANNOT_SET_TTL, "ping: can't set unicast time-to-live: %s",
			strerror(errno));
	}
}

static void	recv_msg(void)
{
	struct iovec		io;
	struct msghdr		msg;
	struct sockaddr_in	recv;
	unsigned char		recv_buf[RECV_BUF_SZ];
	ssize_t				received;

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
	received = recvmsg(g_ft_ping.sock, &msg, 0);
	if (received < 0)
	{
		if (errno != EINTR)
			dlog("recvmsg error: %d, %s", errno, strerror(errno));
	}
	else
		parse_ipv4(recv_buf, received);
}

void	process_ping(void)
{
	inet_ntop(AF_INET, &g_ft_ping.sin_addr,
		g_ft_ping.addr, sizeof(g_ft_ping.addr) - 1);
	dprintf(1, "PING %s (%s) %d(%lu) bytes of data.\n", g_ft_ping.host,
		g_ft_ping.addr, g_ft_ping.packet_size,
		g_ft_ping.packet_size + ICMP_PACKET_SZ + sizeof(struct ip));
	dlog("host: %s", g_ft_ping.addr);
	prepare_socket();
	gettimeofday(&g_ft_ping.start_time, NULL);
	send_icmp();
	while ((g_ft_ping.packets_recv + g_ft_ping.packets_loss)
		!= g_ft_ping.count_total)
	{
		if (g_ft_ping.sock != -1)
			recv_msg();
	}
	print_stat();
	exit(0);
}
