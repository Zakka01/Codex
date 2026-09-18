/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:36:36 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/18 19:09:23 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int get_coder(t_coder *coder)
{
    int chosen_coder;
    int i;

    i = 0;
    chosen_coder = coder->data->queue[0];
    while (i < coder->data->queue_size - 1)
    {
        coder->data->queue[i] = coder->data->queue[i + 1];
        i++;
    }
    coder->data->queue_size--;
    return (chosen_coder);
}

void work(t_coder *coder)
{
    printf("%lu %d is compiling\n", get_time_ms() - coder->data->start_time, coder->id + 1);
    usleep(coder->data->time_to_compile * 1000);
    printf("%lu %d is debugging\n", get_time_ms() - coder->data->start_time, coder->id + 1);
    usleep(coder->data->time_to_debug * 1000);
    printf("%lu %d is refactoring\n", get_time_ms() - coder->data->start_time, coder->id + 1);
    usleep(coder->data->time_to_refactor * 1000);
}

int scheduler_fifo(t_coder *coder)
{
    int n_compiles;
    int chosen;

    n_compiles = coder->data->number_of_compiles;
    while (n_compiles > 0)
    {
        pthread_mutex_lock(&coder->data->scheduler_lock);

        while ((coder->data->current_coder != coder->id))
            pthread_cond_wait(&coder->data->scheduler_cond,
                              &coder->data->scheduler_lock);
                              
        chosen = get_coder(coder);
        pthread_mutex_unlock(&coder->data->scheduler_lock);

        if (acquire_dongles(coder) == 1)
        {
            work(coder);
            release_dongles(coder);
        }
        
        pthread_mutex_lock(&coder->data->scheduler_lock);

        append_queue(&coder->data->coders[chosen]);
        coder->data->current_coder = coder->data->queue[0];

        pthread_cond_broadcast(&coder->data->scheduler_cond);
        pthread_mutex_unlock(&coder->data->scheduler_lock);

        n_compiles--;
    }

    return (0);
}

int scheduler_edf(t_coder *coder)
{
    (void)coder;
    return 0;
}