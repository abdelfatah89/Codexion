/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   init.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/14 15:12:46 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/14 15:12:46 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

t_simulator	*init(int ac, char **av)
{
	t_config	*config;
	t_simulator	*sim;

	config = init_config(ac, av);
	if (!config)
		return (NULL);
	sim = init_simulator(config);
	if (!sim)
		return (NULL);
	init_mutex_cond(sim);
	init_threads(sim);
	return (sim);
}

t_simulator	*init_simulator(t_config *config)
{
	t_simulator		*sim;

	sim = malloc(sizeof(t_simulator));
	if (!sim)
		return (NULL);
	sim->config = config;
	sim->start_time = get_time_in_ms();
	sim->stop = false;
	sim->coders = init_coders(config->coder_count, sim);
	sim->dongles = init_dongles(config->coder_count, sim->config->scheduler);
	return (sim);
}

t_config	*init_config(int ac, char **av)
{
	t_config	*config;

	config = parser(ac, av);
	if (!config)
		return (NULL);
	return (config);
}

void	init_threads(t_simulator *sim)
{
	int	i;

	i = 0;
	while (i < sim->config->coder_count)
	{
		pthread_create(
			&sim->coders[i].thread,
			NULL,
			coder_routine,
			&sim->coders[i]);
		i++;
	}
	pthread_create(&sim->monitor, NULL, monitor_routine, &sim);
	i = 0;
	while (i < sim->config->coder_count)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
}

void	init_mutex_cond(t_simulator *sim)
{
	int	i;

	i = 0;
	pthread_mutex_init(&sim->logger_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	while (i < sim->config->coder_count)
	{
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		pthread_cond_init(&sim->dongles[i].cond, NULL);
		i++;
	}
}
