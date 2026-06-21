/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   dongle.c                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/14 10:43:32 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/14 11:03:44 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

t_dongle	*init_dongles(int dongle_count, t_policy p)
{
	int				i;
	t_dongle		*dongles;

	i = 0;
	dongles = malloc(sizeof(*dongles) * dongle_count);
	if (!dongles)
		return (NULL);
	while (i < dongle_count)
	{
		init_dongle(i, &dongles[i], p);
		i++;
	}
	return (dongles);
}

void	init_dongle(int id, t_dongle *dongle, t_policy p)
{
	pthread_cond_init(&dongle->cond, NULL);
	pthread_mutex_init(&dongle->mutex, NULL);
	dongle->id = id + 1;
	dongle->owner = NULL;
	dongle->cooldown_until = 0;
	dongle->next_order = 0;
	dongle->taken = false;
	init_heap(&dongle->queue, p);
}

static bool	is_next(t_dongle *dongle, t_coder *coder)
{
	return (dongle->queue.size > 0
		&& heap_peek(&dongle->queue).coder == coder
		&& dongle->taken == false);
}

bool	take_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;

	pthread_mutex_lock(&dongle->mutex);
	while (!is_stopped(coder->table) && !(is_next(dongle, coder)
			&& get_time_in_ms() >= dongle->cooldown_until))
	{
		if (is_next(dongle, coder))
		{
			ts = abstime_at_ms(dongle->cooldown_until);
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	if (is_stopped(coder->table))
		return (pthread_mutex_unlock(&dongle->mutex), false);
	heap_pop(&dongle->queue);
	dongle->taken = true;
	dongle->owner = coder;
	pthread_mutex_unlock(&dongle->mutex);
	log_state(coder, "TD");
	return (true);
}

void	release_dongle(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->taken = false;
	dongle->owner = NULL;
	dongle->cooldown_until = get_time_in_ms() + coder->table->config->cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
