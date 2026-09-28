/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:47:29 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/28 20:09:04 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int    call_fifo(t_coder *coder)
{
    int     priority;

    pthread_mutex_lock(&coder->data->scheduler_lock);
    priority = coder->data->priority;
    coder->data->priority++;
    heap_push(coder->data->dongles[coder->id].heap, coder->id, priority);
    heap_push(coder->data->dongles[(coder->id + 1) % coder->data->number_of_coders].heap, coder->id, priority);

    pthread_mutex_unlock(&coder->data->scheduler_lock);

    if (scheduler_fifo(coder) == 1)
        return (1);
    return (0);
}

int    call_edf(t_coder *coder)
{
    int             priority;
    unsigned long   deadline;

    pthread_mutex_lock(&coder->data->scheduler_lock);
    coder->deadline = coder->last_action_time + coder->data->time_to_burnout;

    deadline = coder->deadline;
    priority = coder->data->priority;

    coder->data->priority++;
    heap_push_edf(coder->data->dongles[coder->id].heap, coder->id, deadline, priority);
    heap_push_edf(coder->data->dongles[(coder->id + 1) % coder->data->number_of_coders].heap, coder->id, deadline, priority);

    pthread_mutex_unlock(&coder->data->scheduler_lock);

    if (scheduler_edf(coder) == 1)
        return (1);
    return (0);
}

void wait_lone_coder(t_coder *coder)
{
    pthread_mutex_lock(&coder->data->dongles[0].lock);
    print_log(coder, " has taken a dongle\n");
    pthread_mutex_lock(&coder->data->scheduler_lock);
    while (!coder->data->scheduler_over)
        pthread_cond_wait(&coder->data->scheduler_cond, &coder->data->scheduler_lock);
    pthread_mutex_unlock(&coder->data->scheduler_lock);
    pthread_mutex_unlock(&coder->data->dongles[0].lock);
}

void *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    if (coder->data->number_of_coders == 1)
    {
        wait_lone_coder(coder);
        return (NULL);
    }
    if (strcmp(coder->data->scheduler, "fifo") == 0)
    {
        if (call_fifo(coder) == 1)
            return (NULL);
    }
    else
    {
        if (call_edf(coder) == 1)
            return (NULL);
    }
    return (NULL);
}
