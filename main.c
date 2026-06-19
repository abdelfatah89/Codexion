/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/14 11:03:05 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/14 11:03:05 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

int	main(int ac, char **av)
{
	t_simulator		*simulator;

	simulator = init(ac, av);
	if (!simulator)
		return (1);
	clear(simulator);
	free(simulator);
}
