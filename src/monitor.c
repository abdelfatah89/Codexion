/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   monitor.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/15 11:30:48 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/15 11:30:48 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	*monitor_routine(void *arg)
{
	int				i;
	t_logger_args	args;
	t_simulator		*sim;

	sim = (t_simulator *)arg;
	i = 0;
	args.mutex = &sim->logger_mutex;
	args.state = "BO";
	while (!sim->stop)
	{
		while (i < sim->config->coder_count)
		{
			args.coder_id = sim->coders[i].id;
			if (get_time_in_ms() - sim->coders[i].last_compile_start
				>= sim->config->burnout_time)
			{
				args.timestamp = get_time_in_ms() - sim->start_time;
				return (logger(args), exit(0), NULL);
			}
			if (all_reached(sim->coders,
					sim->config->coder_count, sim->config->required_compiles))
				return (NULL);
		}
		i++;
		usleep(1000);
	}
	return (NULL);
}

int	all_reached(t_coder *coders, int coders_count, int count_required)
{
	int	i;

	i = 0;
	while (i < coders_count)
	{
		if (coders[i].compile_count < count_required)
			return (0);
	}
	return (1);
}

void	stop_simulation(t_simulator *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = true;
	pthread_mutex_unlock(&sim->stop_mutex);
}
