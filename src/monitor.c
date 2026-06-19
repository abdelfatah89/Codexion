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

static int	check_burnout(t_simulator *sim, int i)
{
	t_logger_args	args;
	long			elapsed;

	pthread_mutex_lock(&sim->coders[i].mutex);
	elapsed = get_time_in_ms() - sim->coders[i].last_compile_start;
	pthread_mutex_unlock(&sim->coders[i].mutex);
	if (elapsed < sim->config->burnout_time)
		return (0);
	stop_simulation(sim);
	args.sim = sim;
	args.state = "BO";
	args.coder_id = sim->coders[i].id;
	args.timestamp = (int)(get_time_in_ms() - sim->start_time);
	args.fatal = true;
	logger(args);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_simulator	*sim;
	int			i;

	sim = (t_simulator *)arg;
	while (!is_stopped(sim))
	{
		i = 0;
		while (i < sim->config->coder_count)
		{
			if (check_burnout(sim, i))
				return (NULL);
			i++;
		}
		if (all_reached(sim->coders, sim->config->coder_count,
				sim->config->required_compiles))
			return (stop_simulation(sim), NULL);
		usleep(500);
	}
	return (NULL);
}

int	all_reached(t_coder *coders, int coders_count, int count_required)
{
	int	i;
	int	done;

	i = 0;
	while (i < coders_count)
	{
		pthread_mutex_lock(&coders[i].mutex);
		done = (coders[i].compile_count >= count_required);
		pthread_mutex_unlock(&coders[i].mutex);
		if (!done)
			return (0);
		i++;
	}
	return (1);
}

void	stop_simulation(t_simulator *sim)
{
	int	i;

	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = true;
	pthread_mutex_unlock(&sim->stop_mutex);
	i = 0;
	while (i < sim->config->coder_count)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

bool	is_stopped(t_simulator *sim)
{
	bool	stopped;

	pthread_mutex_lock(&sim->stop_mutex);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stopped);
}
