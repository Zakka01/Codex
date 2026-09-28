/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_fifo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:36:36 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/28 20:13:33 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void work(t_coder *coder)
{
    pthread_mutex_lock(&coder->data->scheduler_lock);
    coder->last_action_time = get_time_ms();
    coder->is_compiling = 1;
    pthread_mutex_unlock(&coder->data->scheduler_lock);

    print_log(coder, " is compiling\n");
    usleep(coder->data->time_to_compile * 1000);

    pthread_mutex_lock(&coder->data->scheduler_lock);
    coder->is_compiling = 0;
    coder->compile_count++;
    pthread_mutex_unlock(&coder->data->scheduler_lock);

    if (coder->data->scheduler_over)
        return;

    release_dongles(coder);
    if (coder->data->scheduler_over){
        return;
    }
    print_log(coder, " is debugging\n");
    usleep(coder->data->time_to_debug * 1000);

    if (coder->data->scheduler_over){
        return;
    }

	print_log(coder, " is refactoring\n");
    usleep(coder->data->time_to_refactor * 1000);
    if (coder->data->scheduler_over)
        return;
}

int scheduler_fifo(t_coder *coder)
{
    int n_compiles;

    n_compiles = coder->data->number_of_compiles;
    while (n_compiles > 0)
    {
        pthread_mutex_lock(&coder->data->scheduler_lock);
        while(!coder->data->scheduler_over
            && (coder->id != coder->data->dongles[coder->id].heap->items[0].coder_id
            || coder->id != coder->data->dongles[(coder->id + 1) % coder->data->number_of_coders].heap->items[0].coder_id
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
            heap_push(coder->data->dongles[coder->id].heap, coder->id, coder->data->priority);
            heap_push(coder->data->dongles[(coder->id + 1) % coder->data->number_of_coders].heap, coder->id, coder->data->priority);        
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
    return (0);
}
