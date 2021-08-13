/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/22 18:05:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/22 18:05:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

typedef struct s_icmp_responses{
	int code;
	const char *description;
} t_icmp_responses;

static t_icmp_responses g_icmp_responses[] = {
	{ICMP_ECHOREPLY, "Echo Reply"},
	{ICMP_DEST_UNREACH, "Destination Unreachable"},
	{ICMP_SOURCE_QUENCH, "Source Quench"},
	{ICMP_REDIRECT, "Redirect"},
	{ICMP_ECHO, "Echo Request"},
	{ICMP_TIME_EXCEEDED, "Time Exceeded"},
	{ICMP_PARAMETERPROB, "Parameter Problem"},
	{ICMP_TIMESTAMP, "Timestamp Request"},
	{ICMP_TIMESTAMPREPLY, "Timestamp Reply"},
	{ICMP_INFO_REQUEST, "Information Request"},
	{ICMP_INFO_REPLY, "Information Reply"},
	{ICMP_ADDRESS, "Address Mask Request"},
	{ICMP_ADDRESSREPLY, "Address Mask Reply"}
};

void print_error(struct icmp *pIcmp)
{
	long unsigned i;

	i = 0;
	while (i < sizeof(g_icmp_responses)/sizeof(g_icmp_responses[0]))
	{
		if (g_icmp_responses[i].code == pIcmp->icmp_type)
		{
			err_fmt(ICMP_ERR, "ICMP error: %s", g_icmp_responses[i].description);
		}
		i++;
	}
	err_fmt(ICMP_ERR, "ICMP unknown error");
}
