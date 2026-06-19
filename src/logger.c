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
	int				timestamp;
	int				coder_id;
	char			*state;

	timestamp = args.timestamp;
	coder_id = args.coder_id;
	state = args.state;
	pthread_mutex_lock(args.mutex);
	print_logs(state, coder_id, timestamp);
	pthread_mutex_unlock(args.mutex);
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
