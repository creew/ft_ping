#ifndef FT_PING_H
# define FT_PING_H

#include "libft.h"
void print_usage();

typedef struct s_ft_ping {
	char *host;
	unsigned int delay;
} t_ft_ping;

enum errors {
	CANNOT_SET_SIGNAL,
	UNKNOWN_HOST,
	OK = 0
};

void dlog(char *fmt, ...);

#endif