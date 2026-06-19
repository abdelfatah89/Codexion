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

struct timespec	abstime_after_ms(long ms)
{
	struct timeval	tv;
	struct timespec	ts;
	long			nsec;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + ms / 1000;
	nsec = (long)tv.tv_usec * 1000L + (ms % 1000) * 1000000L;
	ts.tv_sec += nsec / 1000000000L;
	ts.tv_nsec = nsec % 1000000000L;
	return (ts);
}

void	precise_sleep(t_simulator *sim, long ms)
{
	long	start;

	start = get_time_in_ms();
	while (!is_stopped(sim) && get_time_in_ms() - start < ms)
		usleep(200);
}
