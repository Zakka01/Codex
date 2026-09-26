/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 19:42:10 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/26 18:47:43 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

void    init_heap(t_data *data)
{
    data->heap = malloc(sizeof(t_heap));
	if (!data->heap)
		return ;
    data->heap->items = malloc(sizeof(t_request)
			* data->number_of_coders);
    if (!data->heap->items)
        return ;
    data->heap->size = 0;
    data->heap->capacity = data->number_of_coders;
}

void    heap_push(t_heap *heap, int coder_id, int priority)
{
    int         i;
    int         parent;
    t_request   temp;

    if (heap->size >= heap->capacity)
        return;
    
    i = heap->size;
    heap->items[i].coder_id = coder_id;
    heap->items[i].priority = priority;
    heap->size++;

    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (heap->items[i].priority >= heap->items[parent].priority)
            break;
        temp = heap->items[parent];
        heap->items[parent] = heap->items[i];
        heap->items[i] = temp;

        i = parent;
    }
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

		if (left_child < heap->size
			&& heap->items[left_child].priority
			< heap->items[small].priority)
			small = left_child;

		if (right_child < heap->size
			&& heap->items[right_child].priority
			< heap->items[small].priority)
			small = right_child;

		if (small == i)
			break ;

		temp = heap->items[i];
		heap->items[i] = heap->items[small];
		heap->items[small] = temp;

		i = small;
	}
}