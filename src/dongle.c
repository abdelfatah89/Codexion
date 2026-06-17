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

t_dongle	*init_dongles(int dongle_count)
{
	int				i;
	t_dongle		*dongles;

	i = 0;
	dongles = malloc(sizeof(*dongles) * dongle_count);
	if (!dongles)
		return (NULL);
	while (i < dongle_count)
	{
		init_dongle(i, &dongles[i]);
		i++;
	}
	return (dongles);
}

void	init_dongle(int id, t_dongle *dongle)
{
	pthread_cond_init(&dongle->cond, NULL);
	pthread_mutex_init(&dongle->mutex, NULL);
	dongle->id = id + 1;
	dongle->cooldown_until = 0;
	dongle->taken = false;
	dongle->queue.size = 0;
}

void	take_dongle(t_dongle *dongle)
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;

	mutex = dongle->mutex;
	cond = dongle->cond;
	pthread_mutex_lock(&mutex);
	while (dongle->taken == false && dongle->cooldown_until > 0)
	{
		pthread_cond_wait(&cond, &mutex);
	}
	dongle->taken = true;
	pthread_mutex_unlock(&mutex);
}

void	release_dongle(t_coder *coder, char *side)
{
	pthread_mutex_t	mutex;
	t_dongle		*dongle;
	long			cooldown;

	cooldown = coder->table->config->cooldown;
	if (strcmp(side, "left"))
		dongle = &coder->left;
	else if (strcmp(side, "right"))
		dongle = &coder->right;
	mutex = dongle->mutex;
	pthread_mutex_lock(&mutex);
	dongle->taken = false;
	dongle->cooldown_until = get_time_in_ms() + cooldown;
	pthread_mutex_unlock(&mutex);
}
