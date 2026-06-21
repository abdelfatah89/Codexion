/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   timer.c                                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/16 08:40:30 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/18 08:57:01 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

struct timespec	abstime_at_ms(long abs_ms)
{
	struct timespec	ts;

	ts.tv_sec = abs_ms / 1000L;
	ts.tv_nsec = (abs_ms % 1000L) * 1000000L;
	return (ts);
}

void	precise_sleep(t_simulator *sim, long ms)
{
	long	start;

	start = get_time_in_ms();
	while (!is_stopped(sim) && get_time_in_ms() - start < ms)
		usleep(100);
}
