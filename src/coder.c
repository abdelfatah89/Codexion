/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   init_funcs.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/15 08:44:54 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/15 08:44:54 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

t_coder	*init_coders(int coder_count, t_simulator *sim)
{
	int		i;
	t_coder	*coders;

	i = 0;
	coders = malloc(sizeof(*coders) * coder_count);
	if (!coders)
		return (NULL);
	while (i < coder_count)
	{
		init_coder(i, &coders[i], sim);
		i++;
	}
	return (coders);
}

void	init_coder(int id, t_coder *coder, t_simulator *sim)
{
	coder->table = sim;
	coder->id = id + 1;
	coder->right = get_dongle(coder, "right");
	coder->left = get_dongle(coder, "left");
	coder->compile_count = 0;
	coder->last_compile_start = sim->start_time;
}

t_dongle	*get_dongle(t_coder *coder, char *side)
{
	int	s;
	int	count;

	count = coder->table->config->coder_count;
	s = 0;
	if (strcmp(side, "right") == 0)
		s = coder->id % count;
	else if (strcmp(side, "left") == 0)
		s = coder->id - 1;
	return (&coder->table->dongles[s]);
}
