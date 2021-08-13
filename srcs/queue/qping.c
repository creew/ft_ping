/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qping.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklompus <eklompus@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 22:07:00 by eklompus          #+#    #+#             */
/*   Updated: 2021/05/21 22:07:00 by eklompus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_ping_elem	*add_to_queue(t_ping_elem **root, int id)
{
	struct timeval	tv;
	t_ping_elem		*elem;

	gettimeofday(&tv, NULL);
	elem = ft_memalloc(sizeof(t_ping_elem));
	if (elem == NULL)
		err_fmt(NO_MEMORY, "cannot alloc memory for ping elem");
	elem->id = id;
	elem->next = *root;
	ft_memcpy(&elem->send_time, &tv, sizeof(tv));
	*root = elem;
	return (elem);
}

t_ping_elem	*poll_elem(t_ping_elem **root, int id)
{
	t_ping_elem	**prev;
	t_ping_elem	*elem;

	prev = root;
	elem = *root;
	while (elem)
	{
		if (elem->id == id)
		{
			*prev = elem->next;
			return (elem);
		}
		prev = &elem->next;
		elem = elem->next;
	}
	return (NULL);
}
