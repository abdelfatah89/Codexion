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
	if (checker(&arr_int, &schedular_type))
	{
		assing_values(&args, &arr_int, &schedular_type);
		return (args);
	}
	else
		return (NULL);
}

int	checker(int *arr_int, char *schedular_type)
{
	int	i;

	i = 0;
	while (!arr_int[i])
	{
		if (arr_int[i] < 0)
			return (0);
		i++;
	}
	if (strcmp(schedular_type, "fifo") != 0
		&& strcmp(schedular_type, "edf") != 0)
		return (0);
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
