//
// Created by Алексей on 16.05.2021.
//
#include "ft_ping.h"
#include <netinet/ip_icmp.h>

static char *parse_joined_option(int argc, char *const *argv, char *arg, int *i)
{
	if (*arg == 0) {
		if (*i < argc)
		{
			arg = argv[*i];
			*i = *i + 1;
		}
		else
			return NULL;
	}
	else
	{
		while (ft_isspace(*arg))
			arg++;
	}
	return arg;
}

static void parse_option_c(int argc, char *const *argv, char *arg, int *i)
{
	arg = parse_joined_option(argc, argv, arg, i);
	if (arg == NULL)
		err_fmt_usage(OPTION_REQUIRED, "ping: option requires an argument -- 'c'");
	ft_ping.count = ft_strtol(arg, NULL, 10);
	if (ft_ping.count <= 0)
		err_fmt(INVALID_COUNT_OF_PACKETS, "ping: bad number of packets to transmit.");
	ft_ping.count_total = ft_ping.count;
	dlog("count of packets: %d", ft_ping.count);
}

static void parse_option_s(int argc, char **argv, char *arg, int *i)
{
	arg = parse_joined_option(argc, argv, arg, i);
	if (arg == NULL)
		err_fmt_usage(OPTION_REQUIRED, "ping: option requires an argument -- 's'");
	ft_ping.packet_size = ft_strtol(arg, NULL, 10);
	if (ft_ping.packet_size < 0)
		err_fmt(INVALID_COUNT_OF_PACKETS, "ping: illegal negative packet size %d.", ft_ping.packet_size);
	if (ft_ping.packet_size > SEND_BUF_SZ - sizeof(struct icmp))
		err_fmt(INVALID_SIZE_OF_PACKETS, "Error: packet size 65600 is too large. Maximum is %lu", SEND_BUF_SZ - sizeof(struct icmp));
	dlog("size of packets: %d", ft_ping.packet_size);
}

static int parse_options(int argc, char *argv[], char *arg, int *i)
{
	int contin;

	contin = 0;
	if (*arg == 'c')
		parse_option_c(argc, argv, ++arg, i);
	else if (*arg == 's')
		parse_option_s(argc, argv, ++arg, i);
	else if (*arg == 'h')
		print_usage(2);
	else if (*arg == 'v')
	{
		ft_ping.debug_log = 1;
		contin = 1;
	}
	return contin;
}

int parse_argv(int argc, char *argv[])
{
	char *arg;
	int i;

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
			if (ft_ping.host == NULL)
				ft_ping.host = arg;
			else
				print_usage(2);
		}
	}
	return OK;
}