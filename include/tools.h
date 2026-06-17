/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   tools.h                                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/14 11:03:54 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/14 12:21:44 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_H
# define TOOLS_H

# include "models.h"

// Logger
void		logger(t_logger_args *args);
void		print_logs(char *state, int cid, int t);

// Parser Function.
t_config	*parser(int argc, char **argv);
int			checker(int *arr_int, char *schedular_type);
void		assing_values(t_config *args, int *arr_int, char *schedular_type);

// Init
t_simulator	*init(int ac, char **av);
t_config	*init_config(int ac, char **av);
t_simulator	*init_simulator(t_config *config);
void		init_threads(t_simulator *sim);
void		init_mutex_cond(t_simulator *sim);

// Dongle
t_dongle	*init_dongles(int dongle_count);
void		init_dongle(int id, t_dongle *dongle);
void		take_dongle(t_dongle *dongle);
void		release_dongle(t_dongle *dongle, long cooldown);

// Coder
t_coder		*init_coders(int coder_count, t_simulator *sim);
void		init_coder(int id, t_coder *coder, t_simulator *sim);
t_dongle	*get_dongle(t_coder *coder, char *side);

// Cleaner
void		clear(t_simulator *sim);
void		clear_dongles(t_dongle *dongles);
void		clear_coders(t_coder *coders);
void		clear_config(t_config *config);

// Coder Routine
void		coder_routine(t_coder *coder);
void		compile(t_coder *coder);
void		debug(t_coder *coder);
void		refactor(t_coder *coder);

// Monitor Routine
void		monitor_routine(void);

// Timer
long		get_time_in_ms(void);
void		timer(long duration);

#endif
