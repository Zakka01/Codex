/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 19:42:10 by zahrabar          #+#    #+#             */
/*   Updated: 2026/10/02 23:14:54 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void	heap_push(t_heap *heap, int coder_id, int priority)
{
	int			i;
	int			parent;
	t_request	temp;

	if (heap->size >= heap->capacity)
		return ;
	i = heap->size;
	heap->items[i].coder_id = coder_id;
	heap->items[i].priority = priority;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap->items[i].priority >= heap->items[parent].priority)
			break ;
		temp = heap->items[parent];
		heap->items[parent] = heap->items[i];
		heap->items[i] = temp;
		i = parent;
	}
}

void	heap_push_edf(t_heap *heap,
			int coder_id,
			unsigned long deadline,
			int priority)
{
	int			i;
	int			parent;
	t_request	temp;

	if (heap->size >= heap->capacity)
		return ;
	i = heap->size;
	heap->items[i].coder_id = coder_id;
	heap->items[i].priority = priority;
	heap->items[i].deadline = deadline;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap->items[i].deadline > heap->items[parent].deadline)
			break ;
		if (heap->items[i].deadline == heap->items[parent].deadline)
			if (heap->items[i].priority > heap->items[parent].priority)
				break ;
		temp = heap->items[parent];
		heap->items[parent] = heap->items[i];
		heap->items[i] = temp;
		i = parent;
	}
}

void	check_childs(t_heap *heap, int left_child, int right_child, int *small)
{
	if (left_child < heap->size
		&& heap->items[left_child].priority
		< heap->items[*small].priority)
		*small = left_child;
	if (right_child < heap->size
		&& heap->items[right_child].priority
		< heap->items[*small].priority)
		*small = right_child;
}

void	heap_pop(t_heap *heap)
{
	int			i;
	int			small;
	int			left_child;
	int			right_child;
	t_request	temp;

	if (heap->size == 0)
		return ;
	heap->items[0] = heap->items[heap->size - 1];
	heap->size--;
	i = 0;
	while (1)
	{
		small = i;
		left_child = i * 2 + 1;
		right_child = i * 2 + 2;
		check_childs(heap, left_child, right_child, &small);
		if (small == i)
			break ;
		temp = heap->items[i];
		heap->items[i] = heap->items[small];
		heap->items[small] = temp;
		i = small;
	}
}
