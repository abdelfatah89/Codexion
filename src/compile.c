/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   compile.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/15 17:07:47 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/15 17:07:47 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	compile(t_coder *coder)
{
	t_logger_args	*args;
	long			compile_time;
	long			cooldown;

	dongles_checker(&coder);
	cooldown = coder->table->config->cooldown;
	compile_time = coder->table->config->compile_time;
	args->mutex = coder->table->logger_mutex;
	args->state = "C";
	args->coder_id = coder->id;
	coder->last_compile_start = get_time_in_ms();
	logger(args);
	timer(compile_time);
	release_dongle(&coder->left, cooldown);
	release_dongle(&coder->right, cooldown);
}

int	dongles_checker(t_coder *coder)
{
	if (coder->left && coder->right)
		return (1);
	else if (!coder->left && coder->right)
		return (send_request(&coder, "left"), 0);
	else if (coder->left && !coder->right)
		return (send_request(&coder, "right"), 0);
	else
	{
		send_request(&coder, "right");
		send_request(&coder, "left");
		return (0);
	}
}

int	send_request(t_coder *coder, char *side)
{
	t_request		*request;
	t_dongle		*dongle;
	t_logger_args	*args;

	request->coder = &coder;
	args->mutex = coder->table->logger_mutex;
	args->state = "TD";
	args->coder_id = coder->id;
	dongle = get_dongle(&coder, side);
	take_dongle(&dongle);
	logger(args);
}
