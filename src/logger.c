//
// Created by Алексей on 15.05.2021.
//

#include "ft_ping.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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

void err(int code) {
	exit(code);
}

void err_fmt(int code, char *fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	vdprintf(2, fmt, argptr);
	dprintf(2, "\n");
	err(code);
	va_end(argptr);
}