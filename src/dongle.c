/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 15:04:32 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/16 18:54:03 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void release_dongles(t_coder *coder)
{
    coder->data->dongles[coder->dongle_1_id].coder_id = -1;
    coder->data->dongles[coder->dongle_2_id].coder_id = -1;

    pthread_mutex_unlock(&coder->data->dongles[coder->dongle_1_id].lock);
    pthread_mutex_unlock(&coder->data->dongles[coder->dongle_2_id].lock);

    coder->dongle_1_id = -1;
    coder->dongle_2_id = -1;
}

void append_queue(t_coder *coder)
{
    if (coder->dongle_1_id == -1 && coder->dongle_2_id == -1)
    {
        if (coder->data->queue_size < coder->data->number_of_coders)
        {   
            coder->data->queue[coder->data->queue_size] = coder->id;
            printf("C%d added \n", coder->id + 1);
            coder->data->queue_size++;
        }   
    }
}

int acquire_dongles(t_coder *coder)
{
    int i;

    i = 0;
    while (i < coder->data->number_of_coders)
    {
        // pthread_mutex_lock
        i++;
    }
    return (0);
}