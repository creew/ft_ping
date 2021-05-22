/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 22:19:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/21 22:19:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"

# include <netinet/in.h>
# include <sys/time.h>

# define SEND_BUF_SZ (65535)
# define RECV_BUF_SZ (65535)

# define ICMP_PACKET_SZ (8)

typedef struct s_ping_elem
{
	struct s_ping_elem	*next;
	int					id;
	struct timeval		send_time;
}	t_ping_elem;

typedef struct s_rtt
{
	float	min;
	float	max;
	float	total;
	float	mdev;
}	t_rtt;

typedef struct s_ft_ping
{
	char			*host;
	char			debug_log;
	int				count;
	int				packet_size;
	int				interval;
	unsigned int	ttl;

	int				pid;

	int				count_total;
	int				seq;
	int				sock;

	t_ping_elem		*root;
	unsigned int	packets_send;
	unsigned int	packets_recv;
	unsigned int	packets_loss;
	t_rtt			rtt;
	struct timeval	start_time;
	struct in_addr	sin_addr;
	socklen_t		ai_addrlen;
	struct sockaddr	ai_addr;
	char			addr[INET6_ADDRSTRLEN];
}	t_ft_ping;

extern t_ft_ping	g_ft_ping;

enum e_errors
{
	RUN_UNDER_NOT_ROOT,
	CANNOT_SET_SIGNAL,
	CANNOT_SET_TTL,
	UNKNOWN_HOST,
	INVALID_COUNT_OF_PACKETS,
	INVALID_TTL,
	INVALID_INTERVAL,
	OPTION_REQUIRED,
	INVALID_SIZE_OF_PACKETS,
	NO_MEMORY,
	OK = 0
};

void			print_usage(int code);
int				parse_argv(int argc, char *argv[]);
void			dlog(char *fmt, ...);
void			err(int code);
void			err_fmt(int code, char *fmt, ...);
void			err_fmt_usage(int code, char *fmt, ...);
unsigned short	chksum(void *addr, size_t len);
void			print_stat(void);

void			fill_payload(unsigned char *payload, int size);
long			tv_diff_nano(struct timeval *end, struct timeval *start);
long			tv_diff_micro(struct timeval *end, struct timeval *start);
void			parse_ipv4(const unsigned char *recv_buf, ssize_t received);
void			process_ping(void);
void			send_icmp(void);

t_ping_elem		*add_to_queue(t_ping_elem **root, int id);
t_ping_elem		*poll_elem(t_ping_elem **root, int id);

char			*parse_joined_option(int argc, char *const *argv, char *arg,
					int *i);
void			parse_option_c(int argc, char **argv, char *arg, int *i);
void			parse_option_s(int argc, char **argv, char *arg, int *i);
void			parse_option_t(int argc, char **argv, char *arg, int *i);
void			parse_option_i(int argc, char **argv, char *arg, int *i);
#endif
