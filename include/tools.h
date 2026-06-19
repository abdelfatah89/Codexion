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
void			logger(t_logger_args args);
void			print_logs(char *state, int cid, int t);
void			log_state(t_coder *coder, char *state);

// Parser Function.
t_config		*parser(int argc, char **argv);
int				is_not_digit(char *s);
int				s2i_checker(char **args);
int				checker(int *arr_int, char *schedular_type);
void			assing_values(t_config *args, int *arr_int,
					char *schedular_type);

// Init
t_simulator		*init(int ac, char **av);
t_config		*init_config(int ac, char **av);
t_simulator		*init_simulator(t_config *config);
void			init_threads(t_simulator *sim);
void			init_mutex_cond(t_simulator *sim);

// Dongle
t_dongle		*init_dongles(int dongle_count, t_policy p);
void			init_dongle(int id, t_dongle *dongle, t_policy p);
bool			take_dongle(t_dongle *dongle, t_coder *coder);
void			release_dongle(t_dongle *dongle, t_coder *coder);

// Coder
t_coder			*init_coders(int coder_count, t_simulator *sim);
void			init_coder(int id, t_coder *coder, t_simulator *sim);
t_dongle		*get_dongle(t_coder *coder, char *side);

// Cleaner
void			clear(t_simulator *sim);
void			clear_simulator(t_simulator *sim);
void			clear_dongles(t_dongle *dongles, int dongles_count);
void			clear_coders(t_coder *coders, int coders_count);
void			clear_config(t_config *config);

// Coder Routine
void			*coder_routine(void *arg);
void			compile(t_coder *coder);
void			debug(t_coder *coder);
void			refactor(t_coder *coder);

// Compile
void			send_request(t_coder *coder);

// Monitor Routine
void			*monitor_routine(void *arg);
int				all_reached(t_coder *coders,
					int coders_count, int count_required);
void			stop_simulation(t_simulator *sim);
bool			is_stopped(t_simulator *sim);

// Timer
long			get_time_in_ms(void);
struct timespec	abstime_after_ms(long ms);
void			precise_sleep(t_simulator *sim, long ms);

// Heap (priority queue)
void			init_heap(t_heap *heap, t_policy policy);
int				heap_size(t_heap *heap);
void			heap_push(t_heap *heap, t_request request);
t_request		heap_peek(t_heap *heap);
t_request		heap_pop(t_heap *heap);
int				compare_requests(t_request a, t_request b, t_policy p);
void			swap_requests(t_request *a, t_request *b);

#endif
