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
	while (!is_stopped(coder->table))
	{
		compile(coder);
		if (is_stopped(coder->table))
			break ;
		debug(coder);
		if (is_stopped(coder->table))
			break ;
		refactor(coder);
	}
	return (NULL);
}

void	debug(t_coder *coder)
{
	log_state(coder, "D");
	precise_sleep(coder->table, coder->table->config->debug_time);
}

void	refactor(t_coder *coder)
{
	log_state(coder, "R");
	precise_sleep(coder->table, coder->table->config->refactor_time);
}
