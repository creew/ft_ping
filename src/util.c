/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 21:18:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/21 21:18:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	fill_payload(unsigned char *payload, int size)
{
	int	i;
	int	alphabet_size;

	i = 0;
	alphabet_size = 'z' - 'a' + 1;
	while (i < size)
	{
		*payload++ = 'a' + i % alphabet_size;
		i++;
	}
}

long	tv_diff_nano(struct timeval *end, struct timeval *start)
{
	return ((end->tv_sec - start->tv_sec) * 1000000
		+ (end->tv_usec - start->tv_usec));
}

long	tv_diff_micro(struct timeval *end, struct timeval *start)
{
	return ((end->tv_sec - start->tv_sec) * 1000
		+ (end->tv_usec - start->tv_usec) / 1000);
}
