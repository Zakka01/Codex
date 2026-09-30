/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_fifo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:36:36 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/30 23:48:51 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void	finish_compiles(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->scheduler_lock);
	coder->data->coders_finished++;
	if (coder->data->coders_finished == coder->data->number_of_coders)
	{
		coder->data->scheduler_over = 1;
		pthread_cond_broadcast(&coder->data->scheduler_cond);
	}
	pthread_mutex_unlock(&coder->data->scheduler_lock);
}

void	more_compile(t_coder *coder, int n_compiles)
{
	if (n_compiles > 0 && !coder->data->scheduler_over)
	{
		pthread_mutex_lock(&coder->data->scheduler_lock);
		heap_push(coder->data->dongles[coder->id].heap,
			coder->id, coder->data->priority);
		heap_push(coder->data->dongles[(coder->id + 1)
			% coder->data->number_of_coders].heap,
			coder->id, coder->data->priority);
		coder->data->priority++;
		pthread_cond_broadcast(&coder->data->scheduler_cond);
		pthread_mutex_unlock(&coder->data->scheduler_lock);
	}
}

int	do_(t_coder *coder)
{
	if (coder->data->scheduler_over)
	{
		pthread_mutex_unlock(&coder->data->scheduler_lock);
		return (1);
	}
	heap_pop(coder->data->dongles[coder->id].heap);
	heap_pop(coder->data->dongles[(coder->id + 1)
		% coder->data->number_of_coders].heap);
	pthread_mutex_unlock(&coder->data->scheduler_lock);
	return (0);
}

int	scheduler_fifo(t_coder *coder)
{
	int	n_compiles;

	n_compiles = coder->data->number_of_compiles;
	while (n_compiles > 0)
	{
		pthread_mutex_lock(&coder->data->scheduler_lock);
		while (!coder->data->scheduler_over
			&& (coder->id != coder->data->dongles[
					coder->id].heap->items[0].coder_id
				|| coder->id != coder->data->dongles[(coder->id + 1)
					% coder->data->number_of_coders].heap->items[0].coder_id
				|| acquire_dongles(coder) == 1))
			pthread_cond_wait(&coder->data->scheduler_cond,
				&coder->data->scheduler_lock);
		if (do_(coder) == 1)
			return (1);
		work(coder);
		n_compiles--;
		more_compile(coder, n_compiles);
	}
	finish_compiles(coder);
	return (0);
}
