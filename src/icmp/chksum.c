//
// Created by Алексей on 16.05.2021.
//
#include "ft_ping.h"

unsigned short    chksum(char *addr, size_t len)
{
	unsigned int sum;
	unsigned short word;

	sum = 0;
	while (len > 1)
	{
		word = *addr++;
		word |= *addr++ << 8;
		len -= 2;
		sum += word;
	}
	if (len > 0)
		sum += *addr;
	while (sum>>16)
		sum = (sum & 0xffff) + (sum >> 16);

	return (~sum);
}