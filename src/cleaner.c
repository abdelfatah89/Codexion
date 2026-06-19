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
	int	count;

	count = sim->config->coder_count;
	clear_dongles(sim->dongles, count);
	clear_coders(sim->coders, count);
	clear_simulator(sim);
	clear_config(sim->config);
}

void	clear_simulator(t_simulator *sim)
{
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->logger_mutex);
}

void	clear_dongles(t_dongle *dongles, int dongles_count)
{
	int	i;

	i = 0;
	while (i < dongles_count)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		pthread_cond_destroy(&dongles[i].cond);
		i++;
	}
	free(dongles);
}

void	clear_coders(t_coder *coders, int coders_count)
{
	int	i;

	i = 0;
	while (i < coders_count)
	{
		pthread_mutex_destroy(&coders[i].mutex);
		i++;
	}
	free(coders);
}

void	clear_config(t_config *config)
{
	free(config);
}
