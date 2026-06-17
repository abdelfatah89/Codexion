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

void	coder_routine(t_coder *coder)
{
	bool	stop;

	stop = coder->table->stop;
	while (!stop)
	{
		compile(&coder);
		debug(&coder);
		refactor(&coder);
	}
}

void	debug(t_coder *coder)
{
	t_logger_args	*args;
	int				time;

	time = coder->table->config->debug_time;
	args->mutex = coder->table->logger_mutex;
	args->state = "D";
	args->coder_id = coder->id;
	args->timestamp = get_time_in_ms() - coder->table->start_time;
	logger(args);
	timer(time);
}

void	refactor(t_coder *coder)
{
	t_logger_args	*args;
	int				time;

	time = coder->table->config->refactor_time;
	args->mutex = coder->table->logger_mutex;
	args->state = "R";
	args->coder_id = coder->id;
	args->timestamp = get_time_in_ms() - coder->table->start_time;
	logger(args);
	timer(time);
}
