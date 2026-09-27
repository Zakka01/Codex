/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:47:29 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/27 22:21:11 by zahrabar         ###   ########.fr       */
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
    if (scheduler_edf(coder) == 1)
        return (1);
    return (0);
}

void *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
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
