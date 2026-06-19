/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   heap.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/17 16:18:20 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/17 16:18:20 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	init_heap(t_heap *heap, t_policy policy)
{
	heap->size = 0;
	heap->policy = policy;
}

int	heap_size(t_heap *heap)
{
	return (heap->size);
}

void	heap_push(t_heap *heap, t_request request)
{
	if (heap->size >= 2)
		return ;
	heap->requests[heap->size] = request;
	heap->size++;
	if (heap->size == 2 && compare_requests(heap->requests[1],
			heap->requests[0], heap->policy))
		swap_requests(&heap->requests[0], &heap->requests[1]);
}

t_request	heap_pop(t_heap *heap)
{
	t_request	top;

	top = heap->requests[0];
	heap->size--;
	heap->requests[0] = heap->requests[heap->size];
	return (top);
}

t_request	heap_peek(t_heap *heap)
{
	return (heap->requests[0]);
}
