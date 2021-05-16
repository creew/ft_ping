//
// Created by Алексей on 16.05.2021.
//
#include "ft_ping.h"

unsigned short    chksum(void *addr, size_t len)
{
	unsigned int sum;
	unsigned short *data;

	sum = 0;
	data = (unsigned short *)addr;
	while (len > 1)
	{
		sum += *data++;
		len -= 2;
	}
	if (len > 0)
		sum += *(unsigned char *)addr;
	while (sum>>16)
		sum = (sum & 0xffff) + (sum >> 16);
	return (~sum);
}