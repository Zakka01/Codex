/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:47:29 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/26 19:57:19 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int    call_fifo(t_coder *coder)
{
    int     priority;

    pthread_mutex_lock(&coder->data->scheduler_lock);
    priority = coder->data->priority;
    coder->data->priority++;
    heap_push(coder->data->heap, coder->id, priority);
    printf("C%d priority %d\n", coder->id + 1, coder->data->priority);
    printf("\n");
    
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
