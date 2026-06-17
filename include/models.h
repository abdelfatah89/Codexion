/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   models.h                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/14 10:43:13 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/14 11:03:58 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODELS_H
# define MODELS_H

typedef struct s_config
{
	int		coder_count;
	long	burnout_time;
	long	compile_time;
	long	debug_time;
	long	refactor_time;
	long	cooldown;
	int		required_compiles;
	char	*scheduler;
}	t_config;

typedef struct s_logger_args
{
	pthread_mutex_t	*mutex;
	int				timestamp;
	int				coder_id;
	char			*state;
}	t_logger_args;

typedef struct s_simulator
{
	t_config		*config;
	long			start_time;
	bool			stop;
	pthread_mutex_t	*stop_mutex;
	pthread_mutex_t	*logger_mutex;
	pthread_t		monitor;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_simulator;

typedef struct s_request
{
	t_coder	*coder;
	long	order;
	long	deadline;
}	t_request;

typedef struct s_heap
{
	t_request	*requests;
	int			size;
}	t_heap;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	t_dongle	*left;
	t_dongle	*right;
	long		last_compile_start;
	int			compile_count;
	t_simulator	*table;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	*mutex;
	pthread_cond_t	*cond;
	long			cooldown_until;
	bool			taken;
	t_heap			*queue;
}	t_dongle;

#endif
