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
	long			compile_time;
	t_logger_args	args;

	args.coder_id = coder->id;
	args.state = "C";
	args.timestamp = get_time_in_ms() - coder->table->start_time;
	args.mutex = &coder->table->logger_mutex;
	send_request(coder);
	take_dongle(coder->left, coder);
	take_dongle(coder->right, coder);
	logger(args);
	coder->last_compile_start = get_time_in_ms() - coder->table->start_time;
	coder->compile_count++;
	compile_time = coder->table->config->compile_time;
	usleep(compile_time);
	release_dongle(coder->left, coder);
	release_dongle(coder->right, coder);
}

void	send_request(t_coder *coder)
{
	t_request		request;
	t_dongle		*first;
	t_dongle		*second;

	request.coder = coder;
	request.deadline = coder->last_compile_start
		+ coder->table->config->burnout_time;
	if (coder->left->id < coder->right->id)
	{
		first = coder->left;
		second = coder->right;
	}
	else
	{
		first = coder->right;
		second = coder->left;
	}
	pthread_mutex_lock(&first->mutex);
	request.order = first->queue.size + 1;
	heap_push(&first->queue, request);
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	request.order = second->queue.size + 1;
	heap_push(&second->queue, request);
	pthread_mutex_unlock(&second->mutex);
}
