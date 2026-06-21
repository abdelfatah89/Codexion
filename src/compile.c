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

static void	mark_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_in_ms();
	coder->compile_count++;
	pthread_mutex_unlock(&coder->mutex);
	log_state(coder, "C");
}

static void	push_request(t_dongle *dongle, t_coder *coder)
{
	t_request	request;

	request.coder = coder;
	pthread_mutex_lock(&coder->mutex);
	request.deadline = coder->last_compile_start
		+ coder->table->config->burnout_time;
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_lock(&dongle->mutex);
	request.order = dongle->next_order++;
	heap_push(&dongle->queue, request);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	send_request(t_coder *coder)
{
	if (coder->left == coder->right)
		return (push_request(coder->left, coder));
	if (coder->left->id < coder->right->id)
	{
		push_request(coder->left, coder);
		push_request(coder->right, coder);
	}
	else
	{
		push_request(coder->right, coder);
		push_request(coder->left, coder);
	}
}

void	_compile(t_coder *coder, t_dongle *first, t_dongle *second)
{
	mark_compiling(coder);
	precise_sleep(coder->table, coder->table->config->compile_time);
	release_dongle(first, coder);
	release_dongle(second, coder);
}

void	compile(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	send_request(coder);
	if (coder->left == coder->right)
	{
		if (take_dongle(coder->left, coder))
			precise_sleep(coder->table, coder->table->config->burnout_time + 1);
		release_dongle(coder->left, coder);
		return ;
	}
	first = coder->left;
	second = coder->right;
	if (coder->id % 2 == 0)
	{
		first = coder->right;
		second = coder->left;
	}
	if (!take_dongle(first, coder))
		return ;
	if (!take_dongle(second, coder))
		return (release_dongle(first, coder));
	_compile(coder, first, second);
}
