/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 22:16:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/21 22:16:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

t_ft_ping	g_ft_ping;

void	alrm_handler(int sig)
{
	dlog("hi: sig: %d, count: %d", sig, g_ft_ping.count);
	if (g_ft_ping.count > 0)
		g_ft_ping.count--;
	if (g_ft_ping.count != 0)
		send_icmp();
}

void	int_handler(int sig)
{
	dlog("exit: %d", sig);
	print_stat();
	exit(0);
}

int	get_addr(char *addr)
{
	struct addrinfo	*parsed_ai;
	struct addrinfo	hints;
	int				res;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	res = getaddrinfo(addr, NULL, &hints, &parsed_ai);
	if (res == 0)
	{
		ft_memcpy(&g_ft_ping.ai_addr, parsed_ai->ai_addr,
			sizeof(struct sockaddr));
		g_ft_ping.ai_addrlen = parsed_ai->ai_addrlen;
		g_ft_ping.sin_addr
			= ((struct sockaddr_in *)&g_ft_ping.ai_addr)->sin_addr;
		freeaddrinfo(parsed_ai);
	}
	return (res);
}

void	init_ft_ping(void)
{
	ft_memset(&g_ft_ping, 0, sizeof(g_ft_ping));
	g_ft_ping.interval = 1;
	g_ft_ping.ttl = 54;
	g_ft_ping.pid = getpid();
	g_ft_ping.seq = 0;
	g_ft_ping.packet_size = 56;
	g_ft_ping.sock = -1;
	g_ft_ping.count = -1;
	g_ft_ping.count_total = -1;
}

int	main(int argc, char *argv[])
{
	if (getuid() != 0)
		err_fmt(RUN_UNDER_NOT_ROOT, "program must run under root");
	init_ft_ping();
	parse_argv(argc - 1, argv + 1);
	if (g_ft_ping.host == NULL)
		print_usage(2);
	if (get_addr(g_ft_ping.host) != 0)
		err_fmt(UNKNOWN_HOST, "ping: %s: Name or service not known\n",
			g_ft_ping.host);
	if (signal(SIGALRM, alrm_handler) == SIG_ERR)
		err_fmt(CANNOT_SET_SIGNAL, "cannot set signal: %d", SIGALRM);
	if (signal(SIGINT, int_handler) == SIG_ERR)
		err_fmt(CANNOT_SET_SIGNAL, "cannot set signal: %d", SIGINT);
	process_ping();
	return (0);
}
