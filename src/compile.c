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

	send_request(&coder, "left");
	send_request(&coder, "right");
	while (!coder->left || !coder->right)
	{
		if (!coder->left)
			pthread_cond_wait(&coder->left->cond, NULL);
		else
			pthread_cond_wait(&coder->right->cond, NULL);
	}
	compile_time = coder->table->config->compile_time;
	args->mutex = coder->table->logger_mutex;
	args->state = "C";
	args->coder_id = coder->id;
	args->timestamp = get_time_in_ms() - coder->table->start_time;
	coder->last_compile_start = get_time_in_ms() - coder->table->start_time;
	logger(args);
	timer(compile_time);
}

void	send_request(t_coder *coder, char *side)
{
	t_request		*request;
	t_dongle		*dongle;
	long			burnout;

	dongle = get_dongle(&coder, side);
	pthread_mutex_lock(&dongle->mutex);
	burnout = coder->table->config->burnout_time;
	request->coder = &coder;
	request->deadline = coder->last_compile_start + burnout;
	request->order = dongle->queue->size + 1;
	dongle->queue->requests[dongle->queue->size] = *request;
	pthread_mutex_unlock(&dongle->mutex);
}
