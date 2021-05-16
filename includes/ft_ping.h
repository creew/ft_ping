#ifndef FT_PING_H
# define FT_PING_H

#include "libft.h"

#include <netinet/in.h>


typedef struct s_ft_ping {
	char *host;
	int count;
	int count_total;
	int count_managed;
	int pid;
	int seq;
	int sock;
	unsigned int delay;
	int packet_size;
	unsigned int packets_send;
	unsigned int packets_recv;
	unsigned char snd_buf[65535];
	struct in_addr sin_addr;
	socklen_t ai_addrlen;
	struct sockaddr *ai_addr;
	char addr[INET6_ADDRSTRLEN];
} t_ft_ping;

extern t_ft_ping ft_ping;

enum errors {
	CANNOT_SET_SIGNAL,
	UNKNOWN_HOST,
    INVALID_COUNT_OF_PACKETS,
	INVALID_SIZE_OF_PACKETS,
	OK = 0
};

void print_usage(int code);
int parse_argv(int argc, char *argv[]);
void dlog(char *fmt, ...);
void err(int code);
void err_fmt(int code, char *fmt, ...);
unsigned short    chksum(void *addr, size_t len);

#endif