/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 22:16:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/21 22:16:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	dlog(char *fmt, ...)
{
	va_list			argptr;
	int				pid;
	unsigned int	uid;

	if (g_ft_ping.debug_log)
	{
		va_start(argptr, fmt);
		pid = getpid();
		uid = getuid();
		printf("\x1b[31mpid: %d\x1b[0m, \x1b[32muid: %u\x1b[0m ", pid, uid);
		vprintf(fmt, argptr);
		printf("\n");
		va_end(argptr);
	}
}

void	err_fmt(int code, char *fmt, ...)
{
	va_list	argptr;

	va_start(argptr, fmt);
	vdprintf(2, fmt, argptr);
	dprintf(2, "\n");
	exit(code);
	va_end(argptr);
}

void	err_fmt_usage(char *fmt, ...)
{
	va_list	argptr;

	va_start(argptr, fmt);
	vdprintf(2, fmt, argptr);
	dprintf(2, "\n");
	va_end(argptr);
	print_usage(2);
}
