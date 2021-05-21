/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 21:42:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/21 21:42:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>

char	*parse_joined_option(int argc, char *const *argv, char *arg, int *i)
{
	if (*arg == 0)
	{
		if (*i < argc)
		{
			arg = argv[*i];
			*i = *i + 1;
		}
		else
			return (NULL);
	}
	else
	{
		while (ft_isspace(*arg))
			arg++;
	}
	return (arg);
}

static int	parse_options(int argc, char *argv[], char *arg, int *i)
{
	int	contin;

	contin = 0;
	if (*arg == 'c')
		parse_option_c(argc, argv, ++arg, i);
	else if (*arg == 's')
		parse_option_s(argc, argv, ++arg, i);
	else if (*arg == 't')
		parse_option_t(argc, argv, ++arg, i);
	else if (*arg == 'h')
		print_usage(2);
	else if (*arg == 'v')
	{
		g_ft_ping.debug_log = 1;
		contin = 1;
	}
	else if (*arg != 0)
	{
		dprintf(2, "ping: invalid option -- '%c'\n", *arg);
		print_usage(2);
	}
	return (contin);
}

int	parse_argv(int argc, char *argv[])
{
	char	*arg;
	int		i;

	i = 0;
	while (i < argc)
	{
		arg = argv[i++];
		if (*arg == '-')
		{
			while (parse_options(argc, argv, ++arg, &i))
			{
			}
		}
		else
		{
			if (g_ft_ping.host == NULL)
				g_ft_ping.host = arg;
			else
				print_usage(2);
		}
	}
	return (OK);
}
