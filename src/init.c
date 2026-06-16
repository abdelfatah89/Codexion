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
	sim = init_simulator(&config);
	if (!sim)
		return (NULL);
	init_threads(sim);
	return (sim);
}

t_simulator	*init_simulitor(t_config *config)
{
	t_simulator		*sim;
	pthread_mutex_t	*logger_mutex;

	sim = malloc(sizeof(t_simulator));
	if (!sim)
		return (NULL);
	sim->config = config;
	sim->start_time = get_time_in_ms();
	sim->stop;
	sim->stop_mutex;
	sim->logger_mutex = &logger_mutex;
	sim->monitor;
	sim->coders = init_coders(config->coder_count, &sim);
	sim->dongles = init_dongles(config->coder_count);
	return (sim);
}

t_config	*init_config(int ac, char **av)
{
	t_config	*config;

	config = parser(ac, av);
	if (!config)
		return (NULL);
	return (&config);
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
	}
	i = 0;
	while (i < sim->config->coder_count)
	{
		pthread_join(&sim->coders[i].thread, NULL);
	}
}

void	init_monitor(void)
{
	pthread_t	thread;
}
