/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_options_sct.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/22 00:59:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/22 00:59:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>

void	parse_option_c(int argc, char **argv, char *arg, int *i)
{
	arg = parse_joined_option(argc, argv, arg, i);
	g_ft_ping.count = (int)ft_strtol(arg, NULL, 10);
	if (g_ft_ping.count <= 0)
		err_fmt(INVALID_COUNT_OF_PACKETS,
			"ping: bad number of packets to transmit.");
	g_ft_ping.count_total = g_ft_ping.count;
	dlog("count of packets: %d", g_ft_ping.count);
}

void	parse_option_s(int argc, char **argv, char *arg, int *i)
{
	arg = parse_joined_option(argc, argv, arg, i);
	g_ft_ping.packet_size = (int)ft_strtol(arg, NULL, 10);
	if (g_ft_ping.packet_size < 0)
		err_fmt(INVALID_COUNT_OF_PACKETS,
			"ping: illegal negative packet size %d.",
			g_ft_ping.packet_size);
	if (g_ft_ping.packet_size > SEND_BUF_SZ - sizeof(struct icmp))
		err_fmt(INVALID_SIZE_OF_PACKETS,
			"Error: packet size 65600 is too large. Maximum is %lu",
			SEND_BUF_SZ - sizeof(struct icmp));
	dlog("size of packets: %d", g_ft_ping.packet_size);
}

void	parse_option_t(int argc, char **argv, char *arg, int *i)
{
	arg = parse_joined_option(argc, argv, arg, i);
	g_ft_ping.ttl = ft_strtol(arg, NULL, 10);
	if (g_ft_ping.ttl > 255)
		err_fmt(INVALID_TTL,
			"ping: ttl %lu out of range", g_ft_ping.ttl);
	dlog("ttl value is: %du", g_ft_ping.ttl);
}

void	parse_option_i(int argc, char **argv, char *arg, int *i)
{
	arg = parse_joined_option(argc, argv, arg, i);
	g_ft_ping.interval = (int)ft_strtol(arg, NULL, 10);
	if (g_ft_ping.interval < 0 || g_ft_ping.interval > FT_INTMAX / 1000)
		err_fmt(INVALID_INTERVAL, "ping: bad timing interval");
	dlog("interval value is: %d", g_ft_ping.interval);
}
