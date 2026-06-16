/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   cleaner.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/15 08:15:40 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/15 08:15:40 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	clear(t_simulator *sim)
{
	clear_coders(sim->coders);
	clear_dongles(sim->dongles);
	clear_config(&sim->config);
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->logger_mutex);
}

void	clear_dongles(t_dongle *dongles)
{
	int	i;
	int	len;

	i = 0;
	len = sizeof(dongles) / sizeof(t_dongle);
	while (i < len)
	{
		pthread_mutex_destroy(dongles[i].mutex);
		pthread_cond_destroy(dongles[i].cond);
		free(dongles[i].queue);
		i++;
	}
	free(dongles);
}

void	clear_coders(t_coder *coders)
{
	free(&coders);
}

void	clear_config(t_config *config)
{
	free(config);
}
