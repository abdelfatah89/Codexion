/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   heap.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/17 14:10:00 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/17 14:10:00 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	heap_init(t_heap *heap, t_request *buffer, int capacity,
		t_policy policy)
{
	heap->requests = buffer;
	heap->size = 0;
	heap->capacity = capacity;
	heap->policy = policy;
}

int	heap_size(t_heap *heap)
{
	return (heap->size);
}

void	heap_push(t_heap *heap, t_request request)
{
	if (heap->size >= heap->capacity)
		return ;
	heap->requests[heap->size] = request;
	heap->size++;
	sift_up(heap, heap->size - 1);
}

t_request	heap_peek(t_heap *heap)
{
	return (heap->requests[0]);
}

t_request	heap_pop(t_heap *heap)
{
	t_request	top;

	top = heap->requests[0];
	heap->size--;
	heap->requests[0] = heap->requests[heap->size];
	sift_down(heap, 0);
	return (top);
}
