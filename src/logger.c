/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   logger.c                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/14 10:43:41 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/14 11:03:37 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	logger(t_logger_args args)
{
	pthread_mutex_lock(&args.sim->logger_mutex);
	if (args.fatal || !is_stopped(args.sim))
		print_logs(args.state, args.coder_id, args.timestamp);
	pthread_mutex_unlock(&args.sim->logger_mutex);
}

void	log_state(t_coder *coder, char *state)
{
	t_logger_args	args;

	args.sim = coder->table;
	args.state = state;
	args.coder_id = coder->id;
	args.timestamp = (int)(get_time_in_ms() - coder->table->start_time);
	args.fatal = false;
	logger(args);
}

void	print_logs(char *state, int cid, int t)
{
	if (strcmp(state, "TD") == 0)
	{
		printf("%d %d has taken a dongle\n", t, cid);
	}
	else if (strcmp(state, "C") == 0)
	{
		printf("%d %d is compiling\n", t, cid);
	}
	else if (strcmp(state, "D") == 0)
	{
		printf("%d %d is debugging\n", t, cid);
	}
	else if (strcmp(state, "R") == 0)
	{
		printf("%d %d is refactoring\n", t, cid);
	}
	else if (strcmp(state, "BO") == 0)
	{
		printf("%d %d burned out\n", t, cid);
	}
}
