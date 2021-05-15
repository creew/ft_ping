#ifndef FT_PING_H
# define FT_PING_H

#include "libft.h"

#include <netinet/in.h>
void print_usage();

typedef struct s_ft_ping {
	char *host;
	int count;
	unsigned int delay;
	unsigned int packets_send;
	unsigned int packets_recv;
	struct  in_addr sin_addr;
	char addr[INET6_ADDRSTRLEN];
} t_ft_ping;

enum errors {
	CANNOT_SET_SIGNAL,
	UNKNOWN_HOST,
    INVALID_COUNT_OF_PACKETS,
	OK = 0
};

void dlog(char *fmt, ...);
void err(int code);
void err_fmt(int code, char *fmt, ...);

#endif