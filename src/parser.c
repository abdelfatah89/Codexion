/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parser.c                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/14 10:44:06 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/14 11:03:12 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

t_config	*parser(int argc, char **argv)
{
	t_config	*args;
	int			arr_int[7];
	int			i;
	char		*schedular_type;

	if (!s2i_checker(argv))
		return (printf("Invalid arguments\n"), NULL);
	args = malloc(sizeof(t_config));
	if (!args)
		return (NULL);
	if (argc < 9)
		return (free(args), NULL);
	arr_int[0] = atoi(argv[1]);
	arr_int[1] = atoi(argv[2]);
	arr_int[2] = atoi(argv[3]);
	arr_int[3] = atoi(argv[4]);
	arr_int[4] = atoi(argv[5]);
	arr_int[5] = atoi(argv[6]);
	arr_int[6] = atoi(argv[7]);
	schedular_type = argv[8];
	if (checker(arr_int, schedular_type))
		return (assing_values(args, arr_int, schedular_type), args);
	else
		return (NULL);
}

int	is_not_digit(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '\0')
		return (1);
	while (s[i])
	{
		if (s[i] > 57 && s[i] < 48)
			return (1);
		i++;
	}
	return (0);
}

int	s2i_checker(char **args)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (is_not_digit(args[i]))
			return (0);
	}
	return (1);
}

int	checker(int *arr_int, char *schedular_type)
{
	int	i;

	i = 0;
	while (arr_int[i])
	{
		if (arr_int[i] < 0)
			return (0);
		i++;
	}
	if (arr_int[0] < 1)
		return (printf("Invalid number of coders\n"), 0);
	if (strcmp(schedular_type, "fifo") != 0
		&& strcmp(schedular_type, "edf") != 0)
		return (printf("Invalid scheduler type\n"), 0);
	return (1);
}

void	assing_values(t_config *args, int *arr_int, char *schedular_type)
{
	args->coder_count = arr_int[0];
	args->burnout_time = arr_int[1];
	args->compile_time = arr_int[2];
	args->debug_time = arr_int[3];
	args->refactor_time = arr_int[4];
	args->required_compiles = arr_int[5];
	args->cooldown = arr_int[6];
	args->scheduler = schedular_type;
}
