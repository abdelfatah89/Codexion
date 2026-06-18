/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   heap_utils.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/17 14:10:00 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/17 14:10:00 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	compare_requests(t_request a, t_request b, t_policy policy)
{
	if (policy == POLICY_EDF)
	{
		if (a.deadline != b.deadline)
			return (a.deadline < b.deadline);
		if (a.order != b.order)
			return (a.order < b.order);
		return (a.coder->id < b.coder->id);
	}
	return (a.order < b.order);
}

void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
