//
// Created by Алексей on 15.05.2021.
//

#include "ft_ping.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

void dlog(char *fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	int pid = getpid();
	unsigned int uid = getuid();
	printf("\x1b[31mpid: %d\x1b[0m, \x1b[32muid: %u\x1b[0m ", pid, uid);
	vprintf(fmt, argptr);
	printf("\n");
	va_end(argptr);
}