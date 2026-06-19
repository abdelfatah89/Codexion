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

struct timespec	ms_to_timespec(long total_ms)
{
	struct timespec	ts;

	ts.tv_sec = total_ms / 1000;
	ts.tv_nsec = (total_ms % 1000) * 1000000;
	return (ts);
}
