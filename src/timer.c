/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   timer.c                                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/16 08:40:30 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/16 08:40:30 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	cooldown_timer(long duration)
{
	long	start;

	start = get_time_in_ms;
	while (get_time_in_ms() - start < duration)
	{
		pthread_cond_wait();
	}
}
