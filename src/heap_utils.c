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

void	sift_up(t_heap *heap, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!compare_requests(heap->requests[i], heap->requests[parent],
				heap->policy))
			break ;
		swap_requests(&heap->requests[i], &heap->requests[parent]);
		i = parent;
	}
}

void	sift_down(t_heap *heap, int i)
{
	int	best;
	int	left;
	int	right;

	best = i;
	left = 2 * i + 1;
	right = 2 * i + 2;
	if (left < heap->size && compare_requests(heap->requests[left],
			heap->requests[best], heap->policy))
		best = left;
	if (right < heap->size && compare_requests(heap->requests[right],
			heap->requests[best], heap->policy))
		best = right;
	if (best != i)
	{
		swap_requests(&heap->requests[i], &heap->requests[best]);
		sift_down(heap, best);
	}
}
