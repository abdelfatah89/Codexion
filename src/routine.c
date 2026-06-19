/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   routine.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/15 11:18:43 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/15 11:18:43 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (1)
	{
		pthread_mutex_lock(&coder->table->stop_mutex);
		if (coder->table->stop)
		{
			pthread_mutex_unlock(&coder->table->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->table->stop_mutex);
		compile(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}

void	debug(t_coder *coder)
{
	t_logger_args	args;
	int				time;

	time = coder->table->config->debug_time;
	args.mutex = &coder->table->logger_mutex;
	args.state = "D";
	args.coder_id = coder->id;
	args.timestamp = get_time_in_ms() - coder->table->start_time;
	logger(args);
	usleep(coder->table->config->debug_time);
}

void	refactor(t_coder *coder)
{
	t_logger_args	args;
	int				time;

	time = coder->table->config->refactor_time;
	args.mutex = &coder->table->logger_mutex;
	args.state = "R";
	args.coder_id = coder->id;
	args.timestamp = get_time_in_ms() - coder->table->start_time;
	logger(args);
	usleep(coder->table->config->refactor_time);
}
