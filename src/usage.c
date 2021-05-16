#include "libft.h"

#include <stdlib.h>

void print_usage(int code) {
	ft_putendl("Usage: ping [-vh] [-c count] host\n"
			   "[-s packetsize]");
	exit(code);
}
