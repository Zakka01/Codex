/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_fifo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:36:36 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/20 22:18:58 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int *get_coder(t_coder *coder)
{
    int *chosen_coder;
    int i;

    i = 0;
    chosen_coder = malloc(sizeof(int));
    chosen_coder[0] = coder->data->queue[0];

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
    coder->last_action_time = get_time_ms();

    printf("%lu %d is compiling\n", get_time_ms() - coder->data->start_time, coder->id + 1);
    usleep(coder->data->time_to_compile * 1000);

    release_dongles(coder);

    printf("%lu %d is debugging\n", get_time_ms() - coder->data->start_time, coder->id + 1);
    usleep(coder->data->time_to_debug * 1000);

    printf("%lu %d is refactoring\n", get_time_ms() - coder->data->start_time, coder->id + 1);
    usleep(coder->data->time_to_refactor * 1000);
}

int *coder_turn(t_coder *coder)
{
    int *chosen;

    pthread_mutex_lock(&coder->data->scheduler_lock);

    while (coder->data->current_coder1 != coder->id
            && coder->data->current_coder2 != coder->id
            && !coder->data->scheduler_over)
        pthread_cond_wait(&coder->data->scheduler_cond,
                            &coder->data->scheduler_lock);

    // if one of the coder burnout scheduler over = 1
    if (coder->data->scheduler_over)
    {
        pthread_mutex_unlock(&coder->data->scheduler_lock);
        return (NULL);
    }
    // check burnout
    if (get_time_ms() - coder->last_action_time >= coder->data->time_to_burnout)
    {   
        printf("%lu %d burnout\n", get_time_ms() - coder->data->start_time, coder->id + 1);
        coder->data->scheduler_over = 1;
        pthread_cond_broadcast(&coder->data->scheduler_cond);
        pthread_mutex_unlock(&coder->data->scheduler_lock);
        return (NULL);
    }
    chosen = get_coder(coder);
    pthread_mutex_unlock(&coder->data->scheduler_lock);

    return (chosen);
}

int scheduler_fifo(t_coder *coder)
{
    int n_compiles;
    int *chosen;
    int flag;

    n_compiles = coder->data->number_of_compiles;
    while (n_compiles > 0)
    {
        chosen = coder_turn(coder);
        if (chosen == NULL)
            return (1);

        if (coder->id == coder->data->current_coder1)
            flag = 0;
        else
            flag = 1;

        if (acquire_dongles(&coder->data->coders[chosen[0]], flag) == 1)
            work(&coder->data->coders[chosen[0]]);

        pthread_mutex_lock(&coder->data->scheduler_lock);
        append_queue(&coder->data->coders[chosen[0]]);
        coder->data->done_count++;
        if (coder->data->done_count == 2)
        {
            coder->data->current_coder1 = coder->data->queue[0];
            coder->data->current_coder2 = coder->data->queue[1];
            coder->data->done_count = 0;
            pthread_cond_broadcast(&coder->data->scheduler_cond);
        }
        pthread_mutex_unlock(&coder->data->scheduler_lock);

        n_compiles--;
    }
    return (0);
}
