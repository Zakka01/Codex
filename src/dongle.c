/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 15:04:32 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/16 15:05:36 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void release_dongles(t_coder *coder)
{
    pthread_mutex_unlock(&coder->data->dongles[coder->dongle_1_id].lock);
    pthread_mutex_unlock(&coder->data->dongles[coder->dongle_2_id].lock);
    coder->data->dongles[coder->dongle_1_id].coder_id = -1;
    coder->data->dongles[coder->dongle_2_id].coder_id = -1;
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
            printf("queue = [coder C%d]\n", coder->id);
            coder->data->queue_size++;
        }   
    }
}

int acquire_dongles(t_coder *coder)
{
    int i;

    i = 0;
    while (i < coder->data->number_of_coders){
        if (pthread_mutex_lock(&coder->data->dongles[i].lock) == 0){
            if (pthread_mutex_lock(&coder->data->dongles[(i + 1) % coder->data->number_of_coders].lock) == 0){
                // mark as owned
                coder->dongle_1_id = i;
                coder->dongle_2_id = (i + 1) % coder->data->number_of_coders;
                coder->data->dongles[i].coder_id = coder->id;
                coder->data->dongles[(i + 1) % coder->data->number_of_coders].coder_id = coder->id;
                printf("0 %d has taken a dongle\n", coder->id + 1);
                printf("0 %d has taken a dongle\n", coder->id + 1);
                return (1);
            }
            else {
                pthread_mutex_unlock(&coder->data->dongles[i].lock);
            }
        }
        i++;   
    }
    return (0);
}