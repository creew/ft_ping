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

static int parse_option_c(int argc, char *const *argv, char *arg, int i)
{
	arg = parse_joined_option(argc, argv, arg, &i);
	if (arg == NULL || ft_safe_atoi(arg, &ft_ping.count) != FT_ATOI_OK ||
		ft_ping.count <= 0)
		err_fmt(INVALID_COUNT_OF_PACKETS,
				"invalid count of packets to transmit: '%s'", arg);
	ft_ping.count_total = ft_ping.count;
	dlog("count of packets: %d", ft_ping.count);
	return i;
}

static int parse_option_s(int argc, char **argv, char *arg, int i)
{
	arg = parse_joined_option(argc, argv, arg, &i);
	if (arg == NULL)
		err_fmt(INVALID_SIZE_OF_PACKETS, "empty packet size");
	int res = ft_safe_atoi(arg, &ft_ping.packet_size);
	if (res != FT_ATOI_OK) {
		err_fmt(INVALID_SIZE_OF_PACKETS, "invalid packet size");
	}
	if (ft_ping.packet_size < 0)
		err_fmt(INVALID_SIZE_OF_PACKETS, "invalid negative size: %s", arg);
	if (ft_ping.packet_size > SEND_BUF_SZ - sizeof(struct icmp))
		err_fmt(INVALID_SIZE_OF_PACKETS,
				"packet size cannot be greater than %d", SEND_BUF_SZ - sizeof(struct icmp));
	dlog("size of packets: %d", ft_ping.packet_size);
	return i;
}

static int parse_options(int argc, char *argv[], char *arg, int i)
{
	if (*arg == 'c')
		i = parse_option_c(argc, argv, ++arg, i);
	else if (*arg == 's')
		i = parse_option_s(argc, argv, ++arg, i);
	else if (*arg == 'h')
		print_usage(2);
	return i;
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
			i = parse_options(argc, argv, ++arg, i);
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