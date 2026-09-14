/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:47:29 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/14 20:51:01 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"
#include <stdio.h>

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
            // printf("queue = [coder C%d]\n", coder->id);
            coder->data->queue_size++;
        }   
    }
}

int acquire_dongles(t_coder *coder)
{
    int i;

    i = 0;
    while (i < coder->data->number_of_coders){
        if (pthread_mutex_trylock(&coder->data->dongles[i].lock) == 0){
            if (pthread_mutex_trylock(&coder->data->dongles[(i + 1) % coder->data->number_of_coders].lock) == 0){
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
    append_queue(coder);
    return (0);
}

void *coder_routine(void *coder)
{
    t_coder *coder1;
    int     res;

    coder1 = (t_coder *)coder;
    res = acquire_dongles(coder1);

    if (res == 1)
    {
        printf("0 %d is compiling\n", coder1->id + 1);
        usleep(coder1->data->time_to_compile * 1000);
        
        release_dongles(coder1);
        
        printf("0 %d is debugging\n", coder1->id + 1);
        usleep(coder1->data->time_to_debug * 1000);
        printf("0 %d is refactoring\n", coder1->id + 1);
        usleep(coder1->data->time_to_refactor * 1000);
    }
        
    return (NULL);
}