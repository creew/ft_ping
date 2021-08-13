/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ipv4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 21:16:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/21 21:16:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#include <arpa/inet.h>
#include <stdio.h>

static void	calc_rtt(float time)
{
	if (g_ft_ping.packets_recv == 0)
	{
		g_ft_ping.rtt.min = time;
		g_ft_ping.rtt.max = time;
	}
	else
	{
		if (time < g_ft_ping.rtt.min)
			g_ft_ping.rtt.min = time;
		if (time > g_ft_ping.rtt.max)
			g_ft_ping.rtt.max = time;
	}
	g_ft_ping.rtt.total += time;
	g_ft_ping.rtt.mdev += time * time;
}

static void	parse_founded_packet(ssize_t received, const struct ip *ip,
								 const struct icmp *icmp, t_ping_elem *elem)
{
	struct timeval	tv;
	char			text_ip[64];
	float			time;

	gettimeofday(&tv, NULL);
	time = (float)tv_diff_nano(&tv, &elem->send_time) / 1000;
	inet_ntop(AF_INET, &ip->ip_src, text_ip, sizeof(text_ip) - 1);
	dprintf(1, "%lu bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms\n",
		received - sizeof(struct ip),
		g_ft_ping.host, text_ip,
		icmp->icmp_hun.ih_idseq.icd_seq, ip->ip_ttl, time);
	calc_rtt(time);
	g_ft_ping.packets_recv++;
}

void	parse_icmp(const unsigned char *recv_buf, ssize_t received,
				const struct ip *ip)
{
	struct icmp	*icmp;
	t_ping_elem	*elem;

	icmp = (struct icmp *)(recv_buf + sizeof(struct ip));
	dlog("type: %d, code: %d, id: %d, seq: %d", icmp->icmp_type,
		 icmp->icmp_code, icmp->icmp_hun.ih_idseq.icd_id,
		 icmp->icmp_hun.ih_idseq.icd_seq);
	if (icmp->icmp_type == ICMP_ECHOREPLY || icmp->icmp_type == ICMP_ECHO)
	{
		if (icmp->icmp_hun.ih_idseq.icd_id == g_ft_ping.pid)
		{
			elem = poll_elem(&g_ft_ping.root, icmp->icmp_hun.ih_idseq.icd_seq);
			if (elem != NULL)
				parse_founded_packet(received, ip, icmp, elem);
			ft_memdel((void **)&elem);
		}
	} else {
		print_error(icmp);
	}
}

void	parse_ipv4(const unsigned char *recv_buf, ssize_t received)
{
	struct ip	*ip;

	dlog("received %ld bytes", received);
	ip = (struct ip *)recv_buf;
	dlog("sizeof(ip): %d, protocol: %d,"
		 " tos: %d, len: %d, id: %d, off: %d",
		 sizeof(struct ip), ip->ip_p, ip->ip_tos, ntohs(ip->ip_len),
		 ntohs(ip->ip_id), ntohs(ip->ip_off));
	if (ip->ip_p == IPPROTO_ICMP)
	{
		parse_icmp(recv_buf, received, ip);
	}
}
