/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:47:29 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/18 17:15:51 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void start_gate(t_coder *coder)
{
    pthread_mutex_lock(&coder->data->start_lock);
    coder->data->ready_count++;

    if (coder->data->ready_count == coder->data->number_of_coders)
    {   
        coder->data->current_coder = 0;
        pthread_cond_broadcast(&coder->data->start_cond);
    }
    else
    {
        while (coder->data->ready_count < coder->data->number_of_coders)
            pthread_cond_wait(&coder->data->start_cond,
                              &coder->data->start_lock);
    }

    pthread_mutex_unlock(&coder->data->start_lock);
}

void *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    start_gate(coder);
    if (strcmp(coder->data->scheduler, "fifo") == 0)
    {
        scheduler_fifo(coder);
    }
    else{
        scheduler_edf(coder);
    }

    return (NULL);
}