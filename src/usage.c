/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 21:18:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/21 21:18:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>

void	print_usage(int code)
{
	ft_putendl("Usage: ping [-vh] [-c count] [-s packetsize] [-i interval] "
			   "[-t ttl] destination\n");
	exit(code);
}
