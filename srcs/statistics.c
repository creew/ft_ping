/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statistics.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 21:41:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/21 21:41:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#include <math.h>
#include <stdio.h>

void	print_stat(void)
{
	struct timeval	end_time;
	float			avg;
	float			mdev;

	gettimeofday(&end_time, NULL);
	printf("\n--- %s ping statistics ---\n", g_ft_ping.host);
	printf("%d packets transmitted, %d received, %d%% packet loss, time %lums\n",
		g_ft_ping.packets_send, g_ft_ping.packets_recv,
		(100 - g_ft_ping.packets_recv * 100 / g_ft_ping.packets_send),
		tv_diff_micro(&end_time, &g_ft_ping.start_time));
	if (g_ft_ping.packets_recv != 0)
	{
		avg = g_ft_ping.rtt.total / (float)g_ft_ping.packets_recv;
		mdev = g_ft_ping.rtt.mdev / (float)g_ft_ping.packets_recv;
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			g_ft_ping.rtt.min, avg,
			g_ft_ping.rtt.max, sqrtf(mdev - avg * avg));
	}
}
