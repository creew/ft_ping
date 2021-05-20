#ifndef FT_PING_H
# define FT_PING_H

#include "libft.h"

#include <netinet/in.h>
#include <sys/time.h>

#define SEND_BUF_SZ (65535)
#define RECV_BUF_SZ (65535)

typedef struct ping_elem {
	struct ping_elem * next;
	int id;
	struct timeval send_time;
} t_ping_elem;

typedef struct s_ft_ping {
	char *host;

	char debug_log;
	int count;
	int packet_size;
	unsigned int delay;
	int	ttl;

	int pid;

	int count_total;
	int seq;
	int sock;

	t_ping_elem *root;
	unsigned int packets_send;
	unsigned int packets_recv;
	unsigned int packets_loss;
	struct in_addr sin_addr;
	socklen_t ai_addrlen;
	struct sockaddr ai_addr;
	char addr[INET6_ADDRSTRLEN];
} t_ft_ping;

extern t_ft_ping ft_ping;

enum errors {
	CANNOT_SET_SIGNAL,
	UNKNOWN_HOST,
    INVALID_COUNT_OF_PACKETS,
	OPTION_REQUIRED,
	INVALID_SIZE_OF_PACKETS,
	NO_MEMORY,
	OK = 0
};

void print_usage(int code);
int parse_argv(int argc, char *argv[]);
void dlog(char *fmt, ...);
void err(int code);
void err_fmt(int code, char *fmt, ...);
void err_fmt_usage(int code, char *fmt, ...);
unsigned short    chksum(void *addr, size_t len);


t_ping_elem *add_to_queue(t_ping_elem **root, int id);
t_ping_elem *poll_elem(t_ping_elem **root, int id);
#endif