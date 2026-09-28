/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_edf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 17:29:14 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/28 19:00:57 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int scheduler_edf(t_coder *coder)
{
    int             n_compiles;
    unsigned long   deadline;

    n_compiles = coder->data->number_of_compiles;
    while (n_compiles > 0)
    {
        pthread_mutex_lock(&coder->data->scheduler_lock);
        while (!coder->data->scheduler_over
                && (coder->data->dongles[coder->id].heap->items[0].coder_id != coder->id
                || coder->data->dongles[(coder->id + 1) % coder->data->number_of_coders].heap->items[0].coder_id != coder->id
                || acquire_dongles(coder) == 1))
                pthread_cond_wait(&coder->data->scheduler_cond, &coder->data->scheduler_lock);

        if (coder->data->scheduler_over)
        {
            pthread_mutex_unlock(&coder->data->scheduler_lock);
            return (1);
        }

        heap_pop(coder->data->dongles[coder->id].heap);
        heap_pop(coder->data->dongles[(coder->id + 1) % coder->data->number_of_coders].heap);
        pthread_mutex_unlock(&coder->data->scheduler_lock);
        work(coder);
        n_compiles--;

        if (n_compiles > 0 && !coder->data->scheduler_over)
        {
            pthread_mutex_lock(&coder->data->scheduler_lock);
            deadline = coder->last_action_time + coder->data->time_to_burnout;
            heap_push_edf(coder->data->dongles[coder->id].heap, coder->id, deadline, coder->data->priority);
            heap_push_edf(coder->data->dongles[(coder->id + 1) % coder->data->number_of_coders].heap, coder->id, deadline, coder->data->priority);
            coder->data->priority++;
            pthread_cond_broadcast(&coder->data->scheduler_cond);
            pthread_mutex_unlock(&coder->data->scheduler_lock);
        }
    }
    pthread_mutex_lock(&coder->data->scheduler_lock);
    coder->data->coders_finished++;
    if (coder->data->coders_finished == coder->data->number_of_coders)
    {
        coder->data->scheduler_over = 1;
        pthread_cond_broadcast(&coder->data->scheduler_cond);
    }
    pthread_mutex_unlock(&coder->data->scheduler_lock);
    return 0;
}