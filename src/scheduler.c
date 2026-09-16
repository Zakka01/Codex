/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:36:36 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/16 20:46:20 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int get_coder(t_coder *coder)
{
    int chosen_coder;
    int i;

    chosen_coder = coder->data->queue[0];
    i = 0;

    // shift
    while (i < coder->data->queue_size)
    {
        coder->data->queue[i] = coder->data->queue[i + 1];
        i++;
    }
    // reduce queue size
    coder->data->queue_size--;
    return (chosen_coder);
}

void work(t_coder *coder)
{
    printf("%d is compiling\n", coder->id + 1);
    usleep(coder->data->time_to_compile * 1000);
    printf("%d is debugging\n", coder->id + 1);
    usleep(coder->data->time_to_debug * 1000);
    printf("%d is refactoring\n", coder->id + 1);
    usleep(coder->data->time_to_refactor * 1000);
}

int scheduler_fifo(t_coder *coder)
{
    int n_compiles;
    int chosen_coder;
    
    n_compiles = coder->data->number_of_compiles;
    
    while (n_compiles > 0)
    {
        pthread_mutex_lock(&coder->data->scheduler_lock);
        chosen_coder = get_coder(coder);
        pthread_mutex_unlock(&coder->data->scheduler_lock);

        if (acquire_dongles(&coder->data->coders[chosen_coder]) == 1)
        {
            work(&coder->data->coders[chosen_coder]);
            release_dongles(coder);
        }
        
        n_compiles--;
    }

    return (chosen_coder);
}

int scheduler_edf(t_coder *coder)
{
    (void)coder;
    return 0;
}